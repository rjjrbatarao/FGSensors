#include "FGSensors.h"

/**
 * @brief Construct a new FGSensors::FGSensors object
 *
 * @param address
 * @param wire
 */
FGSensors::FGSensors(byte address, TwoWire *wire) : _sensor_address(address), _wire(wire)
{
}

/**
 * @brief Destroy the FGSensors::FGSensors object
 *
 */
FGSensors::~FGSensors()
{
    // nothing to free
}
/**
 * @brief Construct a new FGSensors::begin object
 * start the wire bus
 */
void FGSensors::begin()
{
#if defined(ESP8266) || defined(ESP32)
    _wire->begin(I2C_SDA, I2C_SCL, 400000);
#else
    _wire->begin();
#endif
    _last_timeout = millis();
}
/**
 * @brief
 *
 * @param new_option
 * @return true
 * @return false
 */
int FGSensors::fgSetAddress(uint8_t new_option)
{
    return fgWriteByte(SENS_ADDR_START, new_option);
}

/**
 * @brief
 *
 * @param new_option
 * @return int
 */
int FGSensors::fgSetOption(uint8_t new_option)
{
    return fgWriteByte(OPT_BYTE_START, new_option);
}

/**
 * @brief
 *
 * @param offset
 * @param new_value
 * @return int
 */
int FGSensors::fgSetCalibration(uint8_t offset, float new_value)
{
    if (offset == OPT_BYTE_START || offset == SENS_ADDR_START)
    {
        return 0; // use fgSetOption or fgSetAddress
    }
    size_t siz;
    siz = sizeof(float);
    uint8_t new_val[siz];
    memcpy(new_val, &new_value, siz);
    return fgWriteBytes(offset, new_val);
}

/**
 * @brief keep register buffer filled
 *
 * @return true
 * @return false
 */
bool FGSensors::fgUpdate(uint16_t interval)
{
    if (_last_timeout == 0)
    {
        return false;
    }
    if (interval < IIC_MIN_INTERVAL)
    {
        interval = IIC_MIN_INTERVAL;
    }
    if (millis() - _last_timeout > interval)
    {
        _last_timeout = millis();
        byte n_receive = _wire->requestFrom(_sensor_address, IIC_REGISTER_LENGTH);
        if (n_receive == IIC_REGISTER_LENGTH)
        {
            for (int i = 0; i < n_receive; i++)
            {
                char y = _wire->read();
                _sensor_register[i] = (uint8_t)y;
            }
            if (FGCrc16(_sensor_register, n_receive - 2) != (uint16_t)((_sensor_register[n_receive - 1] << 8) | _sensor_register[n_receive - 2]))
            {
                _read_available = false;
            }
            else
            {
                _read_available = true;
            }
            return _read_available;
        }
        _read_available = false;
        return false;
    }
    else
    {
        _read_available = false;
        return false;
    }
}

/**
 * @brief
 *
 * @param offset
 * @return float
 */
float FGSensors::fgReadValue(uint8_t offset)
{
    if (offset == OPT_BYTE_START || offset == SENS_ADDR_START)
    {
        return fgReadByte(offset);
    }

    return fgReadBytes(offset);
}

/**
 * @brief
 *
 * @param offset
 * @param new_data
 * @return int
 */
int FGSensors::fgWriteByte(uint8_t offset, byte new_data)
{
    _wire->beginTransmission(_sensor_address);
    _wire->write(offset);
    _wire->write(new_data);
    int error = _wire->endTransmission();
    /**
     * @brief for compat old firmware
     * register realignment
     */
#ifdef IIC_OLD_IMPL
    _wire->beginTransmission(_sensor_address);
    _wire->write(0x00);
    _wire->endTransmission();
#endif
    return error;
}

/**
 * @brief
 *
 * @param offset
 * @param new_data
 * @return int
 */
int FGSensors::fgWriteBytes(uint8_t offset, uint8_t *new_data)
{
    _wire->beginTransmission(_sensor_address);
    _wire->write(offset);
    for (int i = 0; i < 4; i++)
    {
        _wire->write(new_data[i]);
    }
    int error = _wire->endTransmission();

    /**
     * @brief for compat old firmware
     * register realignment
     */
#ifdef IIC_OLD_IMPL
    _wire->beginTransmission(_sensor_address);
    _wire->write(0x00);
    _wire->endTransmission();
#endif
    delay(10);
    return error;
}

/**
 * @brief
 *
 * @param offset
 * @return uint8_t
 */
uint8_t FGSensors::fgReadByte(uint8_t offset)
{
    if (_read_available) // checks for currupted register
    {
        return _sensor_register[offset];
    }
    return -1;
}

/**
 * @brief
 *
 * @param offset
 * @return float
 */
float FGSensors::fgReadBytes(uint8_t offset)
{
    if (_read_available) // checks for currupted register
    {
        memcpy(&_data_bytes[0], _sensor_register + offset, 4);
        memcpy(&_data_float, _data_bytes, sizeof _data_float);
        return _data_float;
    }
    return -1;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool FGSensors::fgIICReady()
{
    uint32_t timeout = millis();
    bool ready = false;
    while ((millis() - timeout < 100) && (!ready))
    {
        _wire->beginTransmission(_sensor_address);
        ready = (_wire->endTransmission() == 0);
    }
    return ready;
}

/**
 * @brief i2c scanner
 *
 */
void FGSensors::fgScan()
{
    DEBUG(" Scanning I2C Addresses\n");
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < 128; i++)
    {
        Wire.beginTransmission(i);
        uint8_t ec = Wire.endTransmission(true);
        if (ec == 0)
        {
            if (i < 16)
                DEBUG("0");
            DEBUG("%x", i);
            cnt++;
        }
        else
            DEBUG("..");
        DEBUG("  ");
        if ((i & 0x0f) == 0x0f)
            DEBUG("\n");
    }
    DEBUG("Scan Completed, ");
    DEBUG("%d", cnt);
    DEBUG(" I2C Devices found.\n");
}

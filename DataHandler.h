#ifndef DATA_HANDLER_HEADER
#define DATA_HANDLER_HEADER


#include "Station.h"
#include <Arduino.h>
#include "dht_nonblocking.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11





class DataHandler {
public:
    // Put public variables in here
    // These are visible outside the class

    // Default Constructor
    DataHandler();

    // Default Destructor
    ~DataHandler(){};


    /// \brief Update the Humidity Sensor This is in a function by itself
    /// becuase it has to be called frequently for proper operation
    void updateDHTSensor();

    
    /// \brief puts all current sensor values in a structure
    /// \param struct of sensor values passed as reference
    /// \returns none
    void getSensorValues(station::sensorData &sensorData);

    /// \brief Checks all the sensors to get their current values
    void checkAllSensors();

    /// \brief runs through all initialization procedures
    /// \param none
    /// \returns none
    bool initializeWeatherStation();


private:
    // put private variables below here
    // These are NOT visible outside the class

    // Initialize the humidity sensor
    DHT_nonblocking humiditySensor{ settings::HUMIDITY_SENSOR_PIN, DHT_TYPE_11};

    // BMP sensor stuff
    // Initialize class for BMP 280 temp and pressure sensor
    Adafruit_BMP280 bmp; // I2C
    Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
    Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
    sensors_event_t temp_event, pressure_event;
    

    // Current sensor values
    float temperature_;
    float pressure_;
    float humidity_;
    float windSpeed_;
    float windSpeedVoltage_;

    // Current values calculated from sensor values
    float dewPoint_;


    // Max number of bad Data points
    int maxBadTempData_;
    int maxBadPressureData_;
    int maxBadRHData_;
    int maxBadWindSpeedData_;



    /// \brief Checks to see if the passed temperature value is within thresholds
    /// \param temp temp value to be checked if its in range
    bool tempInRange(float temp);


    /// \brief Checks to see if the passed pressure value is within thresholds
    /// \param pressure pressure value to be checked if its in range
    bool pressureInRange(float pressure);


    /// \brief Checks to see if the passed humidity value is within thresholds
    /// \param RH humidity value to be checked if its in range
    bool rhInRange(float RH);


    /// \brief Converts the passed pressure value to passed unit value
    /// \param pressure pressure value to be converted, gets taken in as reference
    /// \param units preffered unit value of pressure
    /// \returns none
    float convertPressureUnits(float pressure);


    /// \brief calculates the dew point from the current values in the class
    void calculateDewPoint(float &temp, float &RH);


    /// \brief Reads in the voltage from anemometer and calculates wind speed in m/s
    /// \param none
    /// \return none
    float calculateWindSpeed();

};

#endif

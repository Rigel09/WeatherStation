#include "Station.h"
#include <Arduino.h>
#include "dht_nonblocking.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

// This has to be declared here, no clue why. Wont compile if moved inside class
DHT_nonblocking dht_sensor(settings::HUMIDITY_SENSOR_PIN, DHT_SENSOR_TYPE);


class DataHandler {
public:
    // Put public variables in here
    // These are visible outside the class

    // Default Constructor
    DataHandler();

    

    /// \brief puts all current sensor values in a structure
    /// \param struct of sensor values passed as reference
    /// \returns none
    station::DataValidity getSensorValues(station::sensorData &sensorData);

    /// \brief Checks all the sensors to get their current values
    void checkAllSensors();

    /// \brief Checks to see if the button has been pressed
    /// \returns true if button has been pressed otherwise false
    station::buttonPressType buttonPressOccurance();

    /// \brief runs through all initialization procedures
    /// \param none
    /// \returns none
    void initializeWeatherStation();

    /// \brief Initializes pins for Arduino
    /// \param none
    /// \returns none
    void initialzePins();


    /// \brief Initializes the button value
    /// \param none
    /// \return none
    void initializeButton();

    /// \brief Sets the unit type to be used to US units
    void useUsCustomaryUnits();

    /// \brief Sets the unit type to be used to metric (SI)
    void useSIUnits();

    /// \brief Returns the current unit type being used
    station::unitType getCurrentUnitTypeUsed();

    

private:
    // put private variables below here
    // These are NOT visible outside the class

    
    // button parameters
    int buttonState_;
    bool buttonPreviouslyDown_;
    unsigned long initalButtonDownTime_;
    bool longButtonPress_;
    
    // Current unit type to report values (SI or US)
    station::unitType selectedUnits_;

    // Current sensor values
    float temperature_;
    float pressure_;
    float humidity_;
    float windSpeed_;

    // Current values calculated from sensor values
    float windChill_;
    float heatIndex_;
    float dewPoint_;


    /// \brief Checks to see if the passed temperature value is within thresholds
    /// \param temp temp value to be checked if its in range
    bool tempInRange(float temp);


    /// \brief Checks to see if the passed pressure value is within thresholds
    /// \param pressure pressure value to be checked if its in range
    bool pressureInRange(float pressure);


    /// \brief Checks to see if the passed humidity value is within thresholds
    /// \param RH humidity value to be checked if its in range
    bool rhInRange(float RH);


    /// \brief Converts the passed temperature value to passed unit value
    /// \param temp temp value to be converted, gets taken in as reference
    /// \param units preffered unit value of temp
    /// \returns none
    void convertTempUnits(float &temp, station::unitType units); 


    /// \brief Converts the passed pressure value to passed unit value
    /// \param pressure pressure value to be converted, gets taken in as reference
    /// \param units preffered unit value of pressure
    /// \returns none
    void convertPressureUnits(float &pressure, station::unitType units);


    /// \brief Converts the passed windSpeed value to passed unit value
    /// \param windspeed windspeed value to be converted, gets taken in as reference
    /// \param units preffered unit value of windspeed
    /// \returns none
    void convertWindSpeedUnits(float &windSpeed, station::unitType units);


    /// \brief calculates the dew point from the current values in the class
    void calculateDewPoint();


    /// \brief calculates the heat index from the current values in the class
    void calculateHeatIndex();


    /// \brief calculates the wind chill from the current values in the class
    void calculateWindChill();


    /// \brief after a unit switch has occured, all the currently stored data needs to be converted
    void convertAllInternalData(station::unitType units);

    /// \brief Reads in the voltage from anemometer and calculates wind speed in m/s
    /// \param none
    /// \return none
    void calculateWindSpeed();

};



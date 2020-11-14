#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Contains structures and enumerations needed for the weather station project
// Also contains needed constants
/////////////////////////////////////////////////////////////////////////////////


namespace settings {
    // Stores the last time the sesnors was checked for data
    unsigned long lastSensorCheckTime;

    // Stores the time interval between taking sensor readings in milliseconds
    const unsigned long SENSOR_CHECK_INTERVAL = 100;

    // Stores time interval between updating display in milliseconds
    const unsigned long DISPLAY_UPDATE_INTERVAL = 500;

    // Max time the button can be held down before changing units
    const unsigned long MAX_BUTTON_PRESS_TIME = 5000;

    // Time between screen scroll changes
    const unsigned long SCREEN_SCROLL_CHANGE_INTERVAL = 4000;



    //////////////////////////////////////////////////////////////
    ///// Pin out for Arduino
    //////////////////////////////////////////////////////////////

    // TODO: Set these input pins to correct values
    // Set pins for input from sensors and button
    const int BUTTON_INPUT_PIN = 6;
    const int WIND_SPEED_INPUT_PIN = A0;  // anemometer
    const int TEMPERATURE_INPUT_PIN = 1;
    const int PRESSURE_INPUT_PIN = 5;
    static const int HUMIDITY_SENSOR_PIN = A1;  // humidity sensor

    // Set pins for LCD screen
    const int LCD_SDC_OUTPUT_PIN = 7;
    const int LCD_SDA_OUTPUT_PIN = 8;

    
} // End namespace settings






// Define the namespace
namespace station {
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Use all caps for constants
    // Put any needed constants below here

    
    // TODO: set these thresholds to correct values
    // Stores the limits of the sensors
    struct THRESHOLDS {
        static const int16_t MAXTEMPF = 100;
        static const int16_t MAXTEMPC = 100;
        static const int16_t MINTEMPF = 0;
        static const int16_t MINTEMPC = 0;
        static const int16_t MAXRH = 100;
        static const int16_t MINRH = 0;
        static const int16_t MAXPRESSURE = 100;
        static const int16_t MINPRESSURE = 0;
        static const int16_t MAX_WIND_VOLTAGE = 2;
        static constexpr float MIN_WIND_VOLTAGE = 0.41;
    };

    const float VOLTAGE_CONVERSION = 0.004882814;

    // Put any needed constants above here
    //////////////////////////////////////////////////////////////////////////////////////////////





    //////////////////////////////////////////////////////////////////////////////////////////////
    // Put any structures below here

    // Structure that stores all of the sensor data along with calculated data
    struct sensorData {
        float temperature;
        float humidity;
        float pressure;
        float windChill;
        float heatIndex;
        float dewPoint;
        float windSpeed;

        sensorData(){
            temperature = 0;
            humidity = 0;
            pressure = 0;
            windChill = 0;
            heatIndex = 0;
            dewPoint = 0;
            windSpeed = 0;
        };
    };


    // Put any structures above here
    //////////////////////////////////////////////////////////////////////////////////////////////





    //////////////////////////////////////////////////////////////////////////////////////////////
    // Put any enumerations below here

    

    // Allows the data read in from a sensor to be set to how much error is in it
    enum DataValidity {
        valid,
        invalid,
        possibleError
    };


    // Stores the unit type that should be used
    enum unitType {
        SI,
        US
    };

    enum buttonPressType {
        notPressed,
        quickPress,
        longPress
    };


    // Put any enumerations above here
    //////////////////////////////////////////////////////////////////////////////////////////////

} // end namespace station_common
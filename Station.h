#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Contains structures and enumerations needed for the weather station project
// Also contains needed constants
/////////////////////////////////////////////////////////////////////////////////


namespace settings {
    // Stores the last time the sesnors was checked for data
    unsigned long lastSensorCheckTime;

    // Stores the time interval between taking sensor readings in milliseconds
    const unsigned long SENSOR_CHECK_INTERVAL = 1000;

    // Stores time interval between updating display in milliseconds
    const unsigned long DISPLAY_UPDATE_INTERVAL = 2000;

    // Max time the button can be held down before changing units
    const unsigned long MAX_BUTTON_PRESS_TIME = 5000;



    //////////////////////////////////////////////////////////////
    ///// Pin out for Arduino
    //////////////////////////////////////////////////////////////

    // TODO: Set these input pins to correct values
    // Set pins for input from sensors and button
    const int BUTTON_INPUT_PIN = 2;
    const int WIND_SPEED_INPUT_PIN = 3;
    const int TEMPERATURE_INPUT_PIN = 4;
    const int PRESSURE_INPUT_PIN = 5;
    const int RH_INPUT_PIN = 6;

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
    };
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

        sensorData(){
            temperature = 0;
            humidity = 0;
            pressure = 0;
            windChill = 0;
            heatIndex = 0;
            dewPoint = 0;
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
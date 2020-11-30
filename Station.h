#ifndef STATION_HEADER
#define STATION_HEADER

#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Contains structures and enumerations needed for the weather station project
// Also contains needed constants
/////////////////////////////////////////////////////////////////////////////////


namespace settings 
{
    // Stores the time interval between taking sensor readings in milliseconds
    const unsigned long SENSOR_CHECK_INTERVAL = 100;

    // Stores the time interval between sending data out
    const unsigned long SEND_DATA_INTERVAL = 500;

    // Stores time interval between updating display in milliseconds
    const unsigned long DISPLAY_UPDATE_INTERVAL = 500;

    // Time between screen scroll changes
    const unsigned long SCREEN_SCROLL_CHANGE_INTERVAL = 4000;

    // Max number of bad data points before we throw the sensor out
    const int MAX_BAD_DATA_POINTS = 15;

    // Calibration Offsets
    const float TEMPERATURE_OFFSET = -0.315;    // Temperature offset (degree C)
    const float HUMIDITY_OFFSET = -3;        // Humidity offset (%)
    const float PRESSURE_OFFSET = -1;        // Pressure offset (hPa)
    const float WIND_SPEED_OFFSET = 4.0089;       // Wind Speed Offset (kmh)
    
    //////////////////////////////////////////////////////////////
    ///// Pin out for Arduino
    //////////////////////////////////////////////////////////////

    // Set pins for input from sensors and button
    const int WIND_SPEED_INPUT_PIN = A0;  // anemometer
    static const int HUMIDITY_SENSOR_PIN = 8;  // humidity sensor

    
} // End namespace settings






// Define the namespace
namespace station {
    //////////////////////////////////////////////////////////////////////////////////////////////
    // Use all caps for constants
    // Put any needed constants below here

    // Stores the limits of the sensors
    struct THRESHOLDS {
        static const int16_t MAXTEMPC = 85;
        static const int16_t MINTEMPC = -40;
        static const int16_t MAXRH = 90;
        static const int16_t MINRH = 20;
        static const int16_t MAXPRESSURE = 1100;  // Pressure is in hPa
        static const int16_t MINPRESSURE = 300;
        static const int16_t MAX_WIND_VOLTAGE = 2;
        static constexpr float MIN_WIND_VOLTAGE = 0.41;
    };

    const float VOLTAGE_CONVERSION = 0.004882814;

    // Structure that stores all of the sensor data along with calculated data
    struct sensorData {
        float temperature;
        float humidity;
        float pressure;
        float windSpeedVoltage;
        float heatIndex;
        float dewPoint;
        float windSpeed;

        sensorData(){
            temperature = 0;
            humidity = 0;
            pressure = 0;
            windSpeedVoltage = 0;
            heatIndex = 0;
            dewPoint = 0;
            windSpeed = 0;
        };
    };


    // Put any structures above here
    //////////////////////////////////////////////////////////////////////////////////////////////

} // end namespace station_common

#endif

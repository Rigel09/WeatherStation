#include "DataHandler.h"




////////////////////////////////////////////////////////////////////////////////////////////
// Run the constructor
////////////////////////////////////////////////////////////////////////////////////////////

DataHandler::DataHandler() : 
    temperature_(0),
    pressure_(0),
    humidity_(0),
    dewPoint_(0),
    windSpeed_(0),
    windSpeedVoltage_(0),
    maxBadPressureData_(0),
    maxBadRHData_(0),
    maxBadTempData_(0),
    maxBadWindSpeedData_(0)
{
}



////////////////////////////////////////////////////////////////////////////////////////////
// Define Public Functions
////////////////////////////////////////////////////////////////////////////////////////////
void DataHandler::updateDHTSensor(){
    // temp is a dummy variable and not used
    // temp is required to call function
    float temp;
    float hum;
    if (humiditySensor.measure(&temp, &hum)){
        if (!rhInRange(hum)){
            maxBadRHData_++;
        }
    }

    if (maxBadRHData_ > settings::MAX_BAD_DATA_POINTS){
        humidity_ = 999;
    
    } else {
        humidity_ = hum + settings::HUMIDITY_OFFSET;
    }
}


bool DataHandler::initializeWeatherStation() {
    bool initialized = true;

    // Set the pin as inputs or outputs
    pinMode(settings::HUMIDITY_SENSOR_PIN, INPUT);
    pinMode(settings::WIND_SPEED_INPUT_PIN, INPUT);

    // Make sure the BMP Sensor can be found on the I2C network
    if (!bmp.begin()){
        initialized = false;
        Serial.println("BMP SENSOR COULD NOT BE FOUND!!!!!!!!");
    } else {
        // Initialize the pressure and temp resolution of the BMP Sensor if sensor is found
        // Also Initialize the standby time.....set to 62.5 ms standby between readings aka 16 Hz Sampling Rate
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_63); /* Standby time. */
    }

    return initialized;
}



void DataHandler::checkAllSensors() {
    /////////// Temp Calculations //////////////////
    // Get the temperature
    bmp_temp->getEvent(&temp_event);

    if (!tempInRange(temp_event.temperature)){
        maxBadTempData_++;
    }

    // If the temperature data data is continually bad then dont use it
    if (maxBadTempData_ > settings::MAX_BAD_DATA_POINTS){
        temperature_ = 999;

    } else {
        temperature_ = temp_event.temperature + settings::TEMPERATURE_OFFSET;
    }



    /////////////// Pressure Calculations ////////////////
    // Get the pressure
    bmp_pressure->getEvent(&pressure_event);
    if (!pressureInRange(pressure_event.pressure)){
        maxBadPressureData_++;
    }

    // If the pressure data has to many bad data points then don't use it
    if (maxBadPressureData_ > settings::MAX_BAD_DATA_POINTS){
        pressure_ = 999;
    
    } else {
        pressure_ = pressure_event.pressure +  settings::PRESSURE_OFFSET;
    }

    //////////////// Wind speed Calculations //////////////
    float windspeed = calculateWindSpeed();

    // If the wind speed has to many bad data points then neglect it
    if (maxBadWindSpeedData_ > settings::MAX_BAD_DATA_POINTS){
        windSpeed_ = 999;

    } else {
        
        windSpeed_ = windspeed;
    }


    ///////////// Dew Point Calculations ///////////////////////////
    if ((maxBadTempData_ < settings::MAX_BAD_DATA_POINTS) && (maxBadRHData_ < settings::MAX_BAD_DATA_POINTS)){
        // Can only calculate the dew point if temperature and humidity are valid
        calculateDewPoint(temperature_, humidity_);
    
    } else {
        // Set the dew point to out of range value if it cant be calculated
        // lets us know we have a problem
        dewPoint_ = 999;
    }

    
}

void DataHandler::getSensorValues(station::sensorData &sensorData) {
    // Copy all internal values to the passed data structure
    sensorData.temperature = temperature_;
    sensorData.pressure = pressure_;
    sensorData.windSpeedVoltage = windSpeedVoltage_;
    sensorData.windSpeed = windSpeed_;
    sensorData.humidity = humidity_;
    sensorData.dewPoint = dewPoint_;
}





////////////////////////////////////////////////////////////////////////////////////////////
// Define Private Functions
////////////////////////////////////////////////////////////////////////////////////////////

bool DataHandler::tempInRange(float temp){
    bool tempValid(true);

    if (temp > station::THRESHOLDS::MAXTEMPC || temp < station::THRESHOLDS::MINTEMPC)
        tempValid = false; 

    return tempValid;
}

bool DataHandler::pressureInRange(float pressure){
    bool pressureValid(true);

    if (pressure > station::THRESHOLDS::MAXPRESSURE || pressure < station::THRESHOLDS::MINPRESSURE)
        pressureValid = false; 

    return pressureValid;
}

bool DataHandler::rhInRange(float RH){
    bool RHValid(true);

    if (RH < station::THRESHOLDS::MINRH || RH > station::THRESHOLDS::MAXRH)
        RHValid = false;
    
    return RHValid;
}
                                                                                  

void DataHandler::calculateDewPoint(float &temp, float &RH) {  
    float tempK;
    float DewPointK;
                                              
    // Converts to Kelvin                                                          
    tempK = temp + 273.15;                                                              
    // Uses Temp in Kelvin and Reletive Humidity to find Dew Point                
    DewPointK = tempK - ((100 - RH) / 5);                                          
    // Converts Dew Point from Kelvin to Celcius                                   
    dewPoint_= DewPointK - 273.15;                                                    
}


float DataHandler::calculateWindSpeed() {
    float windspeed = -1;
    int measurementAccumulation = 0;
    
    for (int ii = 0; ii <= 19; ii++){
        // get the voltage value from the anemometer
        measurementAccumulation = measurementAccumulation + analogRead(settings::WIND_SPEED_INPUT_PIN);
    }
    
    // convert the bits read in from analog read to voltage
    float sensorVoltage = measurementAccumulation / 20 * station::VOLTAGE_CONVERSION;

    // Check to make sure the voltage isn't out of bounds
    if (sensorVoltage >= station::THRESHOLDS::MAX_WIND_VOLTAGE) {
        windspeed = 999;
        maxBadWindSpeedData_++;

    } else if  (sensorVoltage <= station::THRESHOLDS::MIN_WIND_VOLTAGE){
        windspeed = 0;

    } else {
        // Using max and min voltage and windspeed 
        windspeed = ( 73.074 * sensorVoltage - 29.262) + settings::WIND_SPEED_OFFSET; 
    }
    return windspeed;
}

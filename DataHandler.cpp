#include "DataHandler.h"


////////////////////////////////////////////////////////////////////////////////////////////
// Run the constructor
////////////////////////////////////////////////////////////////////////////////////////////

DataHandler::DataHandler() : 
    selectedUnits_(station::unitType::US),
    temperature_(0),
    pressure_(0),
    humidity_(0),
    windChill_(0),
    heatIndex_(0),
    dewPoint_(0),
    windSpeed_(0),
    buttonState_(0),
    buttonPreviouslyDown_(false),
    initalButtonDownTime_(0),
    longButtonPress_(false)
{
}



////////////////////////////////////////////////////////////////////////////////////////////
// Define Public Functions
////////////////////////////////////////////////////////////////////////////////////////////

void DataHandler::initialzePins() {
    pinMode(settings::BUTTON_INPUT_PIN, INPUT);
    pinMode(settings::PRESSURE_INPUT_PIN, INPUT);
    pinMode(settings::TEMPERATURE_INPUT_PIN, INPUT);
    pinMode(settings::RH_INPUT_PIN, INPUT);
    pinMode(settings::WIND_SPEED_INPUT_PIN, INPUT);
    pinMode(settings::LCD_SDA_OUTPUT_PIN, OUTPUT);
    pinMode(settings::LCD_SDC_OUTPUT_PIN, OUTPUT);
}

void DataHandler::initializeButton(){
    buttonState_ = digitalRead(settings::BUTTON_INPUT_PIN);
}


void DataHandler::useUsCustomaryUnits(){
    selectedUnits_ = station::unitType::US;
    convertAllInternalData(selectedUnits_);
}


void DataHandler::useSIUnits(){
    selectedUnits_ = station::unitType::SI;
    convertAllInternalData(selectedUnits_);
}


station::unitType DataHandler::getCurrentUnitTypeUsed(){
    return selectedUnits_;
}


station::buttonPressType DataHandler::buttonPressOccurance(){
    station::buttonPressType buttonPressed = station::buttonPressType::notPressed;

    int val = digitalRead(settings::BUTTON_INPUT_PIN);
    delay(10);
    int val2 = digitalRead(settings::BUTTON_INPUT_PIN);


    if (val == val2){  // ensured there was a button press
        
        if (val != buttonState_ || buttonState_ == HIGH){  //button has changed state or is held down

            if(val == LOW){  // Button has been let go of
                // Reset the flag that signifies button is down
                buttonPreviouslyDown_ = false;

                if (longButtonPress_){ // check to see if button press needs to be skipped
                    buttonPressed = station::buttonPressType::longPress;

                } else {
                    buttonPressed = station::buttonPressType::quickPress;
                }
            } else {
                // Button is still held down let us measure the time
                if (!buttonPreviouslyDown_){
                    initalButtonDownTime_ = millis(); // record the time of button press
                    buttonPreviouslyDown_ = true; // set the flags that button is down
                    longButtonPress_ = false; // button has not been down 5 seconds

                } else {
                    // Check to see if button press was longer than set time
                    if (millis() - initalButtonDownTime_ >= settings::MAX_BUTTON_PRESS_TIME){
                        longButtonPress_ = true; // button has been down for 5s
                    }
                }

            }

        } 

    } // End if (val == val2)

    buttonState_ = val;  // record the value of the button
    return buttonPressed;
}





////////////////////////////////////////////////////////////////////////////////////////////
// Define Private Functions
////////////////////////////////////////////////////////////////////////////////////////////
bool DataHandler::tempInRange(float temp){
    bool tempValid(true);

    if (selectedUnits_ == station::unitType::US){
        if (temp > station::THRESHOLDS::MAXTEMPF || temp < station::THRESHOLDS::MINTEMPF)
            tempValid = false; 
    }

    if (selectedUnits_ == station::unitType::SI){
        if (temp > station::THRESHOLDS::MAXTEMPC || temp < station::THRESHOLDS::MINTEMPC)
            tempValid = false; 
    }

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

void DataHandler::convertTempUnits(float &temp, station::unitType units) {
    if (units == station::unitType::SI) {
        // Convert to SI Units aka celcius
        temp = (temp - 32) * 5/9;

    } else { 
        // Need to convert to fahrenheit
        temp = (temp * 9/5) + 32;
    }
}

void DataHandler::convertAllInternalData(station::unitType units){
    convertTempUnits(temperature_, units);
    convertTempUnits(windChill_, units);
    convertTempUnits(heatIndex_, units);
    // These are commented out because they have not been declared yet
    // convertPressureUnits(pressure_, units);
    // convertWindSpeedUnits(windSpeed_, units);
}

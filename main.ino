#include <Arduino.h>
#include "LiquidCrystal_I2C.h"



//////////////////////////////////////////////////////////////////////////////
/////////// This contains the code for the Weather Station ///////////////////
//////////////////////////////////////////////////////////////////////////////


// Stores the last time the sesnors was checked for data
unsigned long lastSensorCheckTime;

// Stores the last time the display was updated
unsigned long lastDisplayChangeTime;

// Create an instance of the main data handler class
DataHandler stationHandler_;

// Initialize the screen
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){
    // Define Setup

    // Initialize the pin modes
    stationHandler_.initialzePins();

    // Initialize the button
    stationHandler_.initializeButton();
    
    // Initiate the time for the last display and sensor change
    lastSensorCheckTime = millis();
    lastDisplayChangeTime = millis();

    // Serial Initialization
    Serial.begin(9600);

    // Initialze the screen
    lcd.init();
    lcd.init();
    lcd.backlight();

    
}


void loop() {
    Serial.println(String(station::THRESHOLDS::MAXPRESSURE));
    
    if ((millis() - lastSensorCheckTime) >= settings::SENSOR_CHECK_INTERVAL){
        // Check Sensors and send data out
        station::sensorData weatherData;

        // These are commented out beacuse they have not been defined
        // If uncommented before definition build will fail
        // stationHandler_.checkAllSensors();
        // stationHandler_.getSensorValues(weatherData);

        lastSensorCheckTime = millis();
    }



    if ((millis() - lastDisplayChangeTime) >= settings::DISPLAY_UPDATE_INTERVAL){
        // Update Display

        lastDisplayChangeTime = millis();
    }

    

    // Check for button press and do stuff
    station::buttonPressType buttonPress = stationHandler_.buttonPressOccurance();

    if (buttonPress == station::buttonPressType::quickPress){
        // Do stuff for quick press

    } else if (buttonPress == station::buttonPressType::longPress) {
        // If button is pressed for long time change the unit type in the weather station
        station::unitType currentType = stationHandler_.getCurrentUnitTypeUsed();

        if (currentType == station::unitType::SI) {
            stationHandler_.useUsCustomaryUnits();
        } else {
            stationHandler_.useSIUnits();
        }

    } else {
        // Button wasn't pressed
        // Handle this here and maybe delete this else
        // may not be needed
    }




    // Check for incoming messages over serial port
    // This should be implemented last after everything is already working
    // Messages to include: Connect, Disconnect, Unit Swap, Set Display update rate, Set sensor check rate

    // All other calculations should be done in DataHandler
}
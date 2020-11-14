#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include <Wire.h> 




//////////////////////////////////////////////////////////////////////////////
/////////// This contains the code for the Weather Station ///////////////////
//////////////////////////////////////////////////////////////////////////////


// Stores the last time the sesnors was checked for data
unsigned long lastSensorCheckTime;

// Stores the last time the display was updated
unsigned long lastDisplayChangeTime;

// Stores the last time the screen was scrolled
unsigned long lastScreenScroll;

// lead row num
int leadRow = 0;

// Create an instance of the main data handler class
DataHandler stationHandler_;

station::sensorData weatherData;

// Initialize the screen
LiquidCrystal_I2C lcd(0x27,20,5);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup(){
    // Define Setup

    // Initialize the pin modes
    stationHandler_.initializeWeatherStation();
    
    // Initiate the time for the last display and sensor change
    lastSensorCheckTime = millis();
    lastDisplayChangeTime = millis();
    lastScreenScroll = millis();

    // Serial Initialization
    Serial.begin(9600);

    // Initialze the screen
    lcd.init();
    lcd.backlight();
    initializeLCD();
}


void loop() {
    
    
    if ((millis() - lastSensorCheckTime) >= settings::SENSOR_CHECK_INTERVAL){
        // Check Sensors and send data out
        stationHandler_.checkAllSensors();
        stationHandler_.getSensorValues(weatherData);
        Serial.println(weatherData.windSpeed);

        lastSensorCheckTime = millis();
    }

    



    if ((millis() - lastDisplayChangeTime) >= settings::DISPLAY_UPDATE_INTERVAL){
        // Update Display
        if ((millis() - lastScreenScroll) >= settings::SCREEN_SCROLL_CHANGE_INTERVAL){
            if (leadRow > 3)   
                leadRow = 0;
            else 
                leadRow++;
        
            lastScreenScroll = millis();
        }
        
        if (leadRow < 4)
            displayTemp(weatherData.temperature, leadRow);


        if (leadRow + 1 < 4)
            displayWindSpeed(weatherData.windSpeed, leadRow + 1);
        else if (leadRow + 1 > 4)
            displayWindSpeed(weatherData.windSpeed, leadRow - 4);

        if (leadRow + 2 < 4)
            displayPressure(weatherData.pressure, leadRow + 2);
        else if (leadRow + 2 > 4)
            displayPressure(weatherData.pressure, leadRow - 3);
            

        if (leadRow + 3 < 4)
            displayHumidity(weatherData.humidity, leadRow + 3);
        else if (leadRow + 3 > 4)
            displayHumidity(weatherData.humidity, leadRow - 2);

        if (leadRow + 4 < 4)
            displayDewPoint(weatherData.dewPoint, leadRow + 4);
        else if (leadRow + 4 > 4)
            displayDewPoint(weatherData.dewPoint, leadRow - 1);
        

        
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


void initializeLCD()
{
    displayTemp(0.0, 0);
    displayWindSpeed(0.0, 1);
    displayPressure(0.0, 2);
    displayHumidity(0.0, 3);
}

void displayTemp(float temperature, int row)
{
    lcd.setCursor( 0, row);
    lcd.print("TEMP:");

    // Write the tempature to screen 
    lcd.setCursor(5,row);
    lcd.print("              ");
    lcd.setCursor(5,row);
    lcd.print(temperature,2);

    lcd.setCursor(19,row);
    
    if (stationHandler_.getCurrentUnitTypeUsed() ==  station::unitType::SI){
        lcd.print("C");
    
    } else {
        lcd.print("F");
    }
}


void displayWindSpeed(float windspeed, int row)
{
    lcd.setCursor(0,row);
    lcd.print("WIND SPEED:");
    lcd.setCursor(11,row);
    lcd.print("      ");
    lcd.setCursor(11,row);
    lcd.print(windspeed,2);
    lcd.setCursor(17,row);

    if (stationHandler_.getCurrentUnitTypeUsed() == station::unitType::SI){
        lcd.print("KMH");

    } else {
        lcd.print("MPH");
    }
}


void displayPressure(float pressure, int row) 
{
    lcd.setCursor(0,row);
    lcd.print("PRESSURE:");
    lcd.setCursor(9,row);
    lcd.print("         ");
    lcd.setCursor(9,row);
    lcd.print(pressure,2);
    

    if (stationHandler_.getCurrentUnitTypeUsed() == station::unitType::SI){
        lcd.setCursor(18, row);
        lcd.print("PA");

    } else {
        lcd.setCursor(17, row);
        lcd.print("PSI");
    }
}


void displayHumidity(float humidity, int row)
{
    lcd.setCursor(0,row);
    lcd.print("HUMIDITY:");
    lcd.setCursor(9,row);
    lcd.print("          ");
    lcd.setCursor(9,row);
    lcd.print(humidity,2);
    lcd.setCursor(19,row);
    lcd.print("%");
}


void displayDewPoint(float dewPoint, int row)
{
    lcd.setCursor( 0, row);
    lcd.print("DEW POINT:");

    // Write the tempature to screen 
    lcd.setCursor(10,row);
    lcd.print("        ");
    lcd.setCursor(10,row);
    lcd.print(dewPoint,2);

    lcd.setCursor(19,row);
    
    if (stationHandler_.getCurrentUnitTypeUsed() ==  station::unitType::SI){
        lcd.print("C");
    
    } else {
        lcd.print("F");
    }
}

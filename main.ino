// This File Contains the code for the weather station 
// MAE 311 Project - Weather Station
// Written by: Joshua Newton, Ryan Free
// Fall 2020
// Measures Temperature (C), Barometric Pressure (hPa), Humidity (%RH), and Wind Speed (kmh)
// Calculates Dew Point (C)


#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include <Wire.h> 
#include "DataHandler.h"
#include "Station.h"



// Stores the last time the sesnors was checked for data
unsigned long lastSensorCheckTime;

// Stores the last time the display was updated
unsigned long lastDisplayChangeTime;

// Stores the last time the screen was scrolled
unsigned long lastScreenScroll;

// Stores the last time data was sent out
unsigned long lastDataSend;

// lead row num
int leadRow = 0;

// Create an instance of the main data handler class
DataHandler stationHandler_;

station::sensorData weatherData;

// Initialize the screen
LiquidCrystal_I2C lcd(0x27,20,5);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup(){
    // Initiate the time for the last display and sensor change
    lastSensorCheckTime = millis();
    lastDisplayChangeTime = millis();
    lastScreenScroll = millis();
    lastDataSend = millis();

    // Serial Initialization
    Serial.begin(9600);

    // Initialze the screen
    lcd.init();
    lcd.backlight();

    // Initialize the weather station
    if (!stationHandler_.initializeWeatherStation()){
        lcd.setCursor(0,0);
        lcd.print("    ERROR!!!!!!");
        lcd.setCursor(0,1);
        lcd.print("   BMP SENSOR WAS   ");
        lcd.setCursor(0,2);
        lcd.print("    NOT FOUND    ");
        lcd.setCursor(0,3);
        lcd.print("   CHECK WIRING");

        for (;;){
            // Do Nothing station failed to initialize
            // Infinite Loop
            Serial.println("NOT INITIALIZED");
            delay(1000);
        }
        
    } else {
        initializeLCD();
    }
}


void loop() {
    // Humidity Sensor needs to be continually updated very fast
    stationHandler_.updateDHTSensor();
    
    
    if ((millis() - lastSensorCheckTime) >= settings::SENSOR_CHECK_INTERVAL){
        // Check Sensors
        stationHandler_.checkAllSensors();
        stationHandler_.getSensorValues(weatherData);

        lastSensorCheckTime = millis();
    }

    if (millis() - lastDataSend > settings::SEND_DATA_INTERVAL){
        // Send data out
        Serial.print(millis());
        Serial.print(":");
        Serial.print(weatherData.temperature);
        Serial.print(":");
        Serial.print(weatherData.pressure);
        Serial.print(":");
        Serial.print(weatherData.humidity);
        Serial.print(":");
        Serial.print(weatherData.windSpeed);
        Serial.print(":");
        Serial.print(weatherData.windSpeedVoltage);
        Serial.print(":");
        Serial.println(weatherData.dewPoint);
        lastDataSend = millis();
    }

    



    if ((millis() - lastDisplayChangeTime) >= settings::DISPLAY_UPDATE_INTERVAL){
        // Update Display

        if ((millis() - lastScreenScroll) >= settings::SCREEN_SCROLL_CHANGE_INTERVAL){
            // Scroll the screen because we have 5 things to display and only 4 rows
            if (leadRow > 3)   
                leadRow = 0;
            else 
                leadRow++;
        
            lastScreenScroll = millis();
        }
        
        if (leadRow < 4)
            displayTemp(weatherData.temperature, leadRow);

        // Put the appriate data on the appropriate lcd row
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
    
    lcd.print("C");
    
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

    lcd.print("KMH");
}


void displayPressure(float pressure, int row) 
{
    lcd.setCursor(0,row);
    lcd.print("PRESSURE:");
    lcd.setCursor(9,row);
    lcd.print("         ");
    lcd.setCursor(9,row);
    lcd.print(pressure,2);
    

    lcd.setCursor(17, row);
    lcd.print("hPa");
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
    
    lcd.print("C");
}

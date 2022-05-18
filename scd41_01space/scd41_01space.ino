//
// SCD4x example
// Customized for the 01Space ESP32-C3 0.42" OLED board
// written by Larry Bank
//
// Displays the SCD41 CO2 sensor values on the built-in 72x40 OLED
//
#include <Wire.h>
#include <OneBitDisplay.h>
#include "SparkFun_SCD4x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD4x

SCD4x mySensor;
ONE_BIT_DISPLAY obd;
// Need to explicitly set the I2C pin numbers on both boards
#ifdef ARDUINO_ARCH_MBED
#define SDA_PIN 22
#define SCL_PIN 23
extern MbedI2C *pWire;
#else
extern TwoWire *pWire;
#define SDA_PIN 5
#define SCL_PIN 6
#endif

void lightSleep(uint64_t time_in_ms)
{
#ifdef HAL_ESP32_HAL_H_
  esp_sleep_enable_timer_wakeup(time_in_ms * 1000);
  esp_light_sleep_start();
#else
  delay(time_in_ms);
#endif
} /* lightSleep() */

void setup()
{
  obd.setI2CPins(SDA_PIN, SCL_PIN);
  obd.I2Cbegin(OLED_72x40);
  obd.allocBuffer();
   //obd.setContrast(50);
  obd.fillScreen(0);
  obd.setFont(FONT_8x8);
  obd.println("Starting...");
  obd.display();
  
  // Start periodic measurements
  if (mySensor.begin(*pWire) == false)
  {
   obd.println("Sensor");
   obd.print("Failure!");
   obd.display();
    while (1) {};
  }
} /* setup() */

void loop()
{
  char szTemp[32];
  // The SCD41 has data ready every 5 seconds in this mode
  if (mySensor.readMeasurement()) // readMeasurement will return true when fresh data is available
  {
      obd.setCursor(0,0);
      sprintf(szTemp, "CO2: %d \n\n", mySensor.getCO2());
      obd.print(szTemp);
      sprintf(szTemp, "Temp: %dC \n\n", (int)mySensor.getTemperature());
      obd.print(szTemp);
      sprintf(szTemp, "Hum: %d%%", (int)mySensor.getHumidity());
      obd.print(szTemp);
      obd.display();
  }
  lightSleep(1000);
} /* loop() */

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
// Need to explicitly set the I2C pin numbers on this C3 board
#define SDA_PIN 5
#define SCL_PIN 6

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
  Wire.begin(SDA_PIN, SCL_PIN);
  obd.setI2CPins(SDA_PIN, SCL_PIN);
  obd.I2Cbegin(OLED_72x40);
  obd.allocBuffer();
//  obd.setContrast(50);
  obd.fillScreen(0);
  obd.setFont(FONT_8x8);
  obd.println("Starting...");
  obd.display();
  // Start periodic measurements
  if (mySensor.begin() == false)
  {
   obd.print("Failure!");
   obd.display();
    while (1) {};
  }
} /* setup() */

void loop()
{
  // The SCD41 has data ready every 5 seconds in this mode
  if (mySensor.readMeasurement()) // readMeasurement will return true when fresh data is available
  {
      obd.setCursor(0,0);
      obd.printf("CO2: %d \n\n", mySensor.getCO2());
      obd.printf("Temp: %dC \n\n", (int)mySensor.getTemperature());
      obd.printf("Hum: %d%%", (int)mySensor.getHumidity());
      obd.display();
  }
  lightSleep(1000);
} /* loop() */

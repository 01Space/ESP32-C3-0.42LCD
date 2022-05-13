# ESP32-C3-0.42PLED 

![image](https://github.com/01Space/ESP32-C3-0.42LCD/blob/main/image/ESP32-C3-0.42LCD.jpg)


![image](https://github.com/01Space/ESP32-C3-0.42LCD/blob/main/image/SCD41_CO2.jpg)


![image](https://github.com/01Space/ESP32-C3-0.42LCD/blob/main/image/SCD41_CO2_sensor.jpg)



# driver

https://zadig.akeo.ie/


# General Flashing Instructions:

Install driver with zadig as mentioned above.
Install Arduino IDE and install espressif resources by adding line "https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json" to Additional Boards Manager URLs in Arduino IDE Files / Preferences.
Via Arduino IDE Tools/Board / Board Manager search for ESP32 and choose Espressif Systems version 2.0.3 to install it.
Install ESP32 Digital RGB LED Drivers and Adafruit NeoPixel via option tools / Manage Libraries in Arduino IDE.
Setup connection to connect to Board ESP32C3 Dev and com port which is assocuiated to USB Serial Device, not jtag (see Device Manager on Win10). Other settings see settings.png.

# Upload sketch from Arduino IDE:

Hold down B button on board, press R button and release again but keep on pressing B, trigger Arduino IDE to upload sketch, keep B pressed until Arduino IDE says that it's connected. Then you can release B button.

Serial works with "CDC enable on boot" set to true.

# Other branches

fivebyfive-by Andy piper
https://github.com/andypiper/fivebyfive

wordle-device-Ciro Cattuto
https://github.com/ccattuto/wordle-device

# esp32c3-bling Series' Articles

https://dev.to/andypiper/series/16659

# Open Source / Contributors


Andy piper (for fivebyfive),

Ciro Cattuto (for wordle-device),

Geek Mom Projects([Sites Using React](https://twitter.com/GeekMomProjects/status/1479210241807900676)  ),

And many many others who haven't been mentioned....

# Contact 01Space
facebook:Jiale Xu

E-mail：759315223@qq.com


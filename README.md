# Undergraduate thesis
Undergraduate thesis embedded project with ESP32 development board and sensors at FEEIT - Skopje. 

The project includes working with:
- BME280 sensors to measure local - indoor temperature, pressure and humidity;
- Wi-Fi to send the measured data to a web server; 
- Bluetooth (BLE) to recieve Wi-Fi credentials and other compensation data from a mobile device;
- 3.2” SPI LCD screen to display the current recorded data;

The project can be extended with another ESP32 board and BME280 sensor measuring outdoor temperature, pressure and humidity and sending the data to the main - master ESP32. If extended, the outdoor system would have its own projected battery power supply that can be charged via the grid or via adequate solar panel built-in on the device.

The project also includes a web based application that will display the recored data and an android application for the bluetooth communication. The project will serve as a weather station and the data collected can be further processed and used to develop prediction and foresee patterns.

# WD5ACP-WiFiPad
Arduino code files for WiFiPad app for use with Kenwood TS890 radio

Guy Zebrick / WD5ACP @ 2020

   This program is for use with the ESP8266 WeMod D1 R2 mini Ardiono board
   connected to a small touchscreen display 2.4″ ILI9341 Color Display TFT with Touch Interface XPT2046

   Configure you WiFi and Host device IP and user settings in the settings.h file before using.

   Progammed menu display and button fucntions are examples. Change as needed to match your application.

   This program connects to a WiFi network and then connects to a host device via IP
   The program exposes 8 configurable push buttons per the touch screen plus a TOP level menu/button 0 used to identify each menu and to rotate to a new screen
   each button has a configuratble lablel (2 lines / 10 chars each) and may have multiple commands assiciated with it.

   Original  program written for use with Kenwood TS890 Host Amateur Radio transceiver.

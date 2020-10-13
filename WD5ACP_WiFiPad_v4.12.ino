#include <ArduinoOTA.h>
#define VER 4.12  // Program Version Number resides here. Change this every time you save - as to a new version number.

/** Guy Zebrick / WD5ACP @ 2020

   This program is for use with the ESP8266 WeMod D1 R2 mini Ardiono board
   connected to a small touchscreen display 2.4″ ILI9341 Color Display TFT with Touch Interface XPT2046

   Configure your WiFi and Host device IP and user settings in the settings.h file before using.

   Progammed menu display and button fucntions are examples. Change as needed to match your application.

   This program connects to a WiFi network and then connects to a host device via IP
   The program exposes 8 configurable push buttons per the touch screen plus a TOP level menu/button 0 used to identify each menu and to rotate to a new screen
   each button has a configuratble label (2 lines / 10 chars each) and may have multiple commands assiciated with it.

   Original  program written for use with Kenwood TS890 Host Amateur Radio transceiver.

   4.08 Fixed Connect buttons / Added WiFi connection menu / Fixed auto Host ID calc
   4.09 Renamed menu screens to numeric / Added voice message menu / Added MR channel menu / added empty WiFi USB config button
   4.10 Added sleep mode timer to turn off backlight LED
   4.11 Added ON AIR indicator with 10 min timer (replaced PS call with a BY call and shortened to every 5 seconds). Added OverTheAir wifi programming
   4.12 Fixed bug in sleep timer (moved bracket)

*/

/*****************************
   Important: see settings.h to configure your settings!!!
 * ***************************/
#include "settings.h"

#include <Arduino.h>tft driver
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <XPT2046_Touchscreen.h>
#include "TouchControllerWS.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/***
   Install the following libraries through Arduino Library Manager
   - Mini Grafx by Daniel Eichhorn
 ***/
#include <MiniGrafx.h>
#include <ILI9341_SPI.h>

// Color names are assigned to a number here. A total of 16 may be listed here, though not all shown are used in the code.
#define MINI_BLACK 0
#define MINI_WHITE 1
#define MINI_YELLOW 2
#define MINI_BLUE 3
#define MINI_NAVY 4
#define MINI_RED  5
#define MINI_GREY  6
#define MINI_GREEN 7
#define MINI_CYAN 8
#define MINI_MAROON 9
#define MINI_DARKRED 10

// defines the colors usable in the paletted 16 color frame buffer -
uint16_t palette[] = {ILI9341_BLACK, // 0
                      ILI9341_WHITE, // 1
                      ILI9341_YELLOW, // 2
                      0x7E3C, // A different shade of blue
                      ILI9341_NAVY,
                      ILI9341_RED,
                      ILI9341_DARKGREY,
                      ILI9341_DARKGREEN,
                      ILI9341_CYAN,
                      ILI9341_MAROON,
                      0xC800         // Custom dark red
                     };

int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 320;
// Limited to 4 colors due to memory constraints
int BITS_PER_PIXEL = 4; // Changed from 2 bits 4 colors in original weather station - now supports up to 16 colors. Some are listed above.

ILI9341_SPI tft = ILI9341_SPI(TFT_CS, TFT_DC);
MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);
TouchControllerWS touchController(&ts);

void calibrationCallback(int16_t x, int16_t y);
CalibrationCallback calibration = &calibrationCallback;

ADC_MODE(ADC_VCC); // Sets up CPU to measure internal voltage - use in the About menu

String loginStatus;
boolean closeHost = false;

char writeCode[] = ";";
char readCode[] = ";";
String putString;
String retString;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
int buttNum = 99;

boolean sleepTimerExp = false;
boolean newSerData;

// Subroutines defined up here so they may be referenced from any other routine -----------------------------------------------------------------------------------------------------------

void drawMenu0();
void drawMenu1();
void drawMenu2();
void drawMenu3();
void drawMenu4();
void drawMenu5();
void drawMenu6();
void drawMenu7();
void drawMenu8();
void drawMenu9();

void recvWithEndMarker();
void showNewData();

void connectHost(); // subroutine to connect to Host - Must be modified per host login requirements
void writeHost(String putString); // subroutine to send command to host
void readHost();  // subrouting to receive data from host

void drawButton(int Num, String Lab1, String Lab2, String Lab3);  // Label 3 only used with top menu button 0
void readButton();

void drawProgress(uint8_t percentage, String text);
void drawWifiQuality();

void drawLabelValue(uint8_t line, String label, String value);

uint16_t screen = 0;
long timerPress;
bool canBtnPress;  // Enables touch screen

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  //Manual Wifi
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  Serial.print("/");
  Serial.println(WIFI_PASS);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (i > 80) i = 0;
    drawProgress(i, "Connecting to WiFi '" + String(WIFI_SSID) + "'");
    i += 10;
    Serial.print(".");
  }
  drawProgress(100, "Connected to WiFi '" + String(WIFI_SSID) + "'");
  Serial.print("Connected...");

  // Added OTA programming from sample code
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

WiFiClient client;  // Connect to client prior to sending data - Initialize at top of program

void setup() { // Initial Setup start here           -------------------------------------------------------------------------------------------------------------------
  Serial.begin(115200);
  Serial.println(TFT_LED);
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);    // HIGH to Turn ON TFT backlight LED (turns off in sleep mode).

  gfx.init();
  gfx.fillBuffer(MINI_BLACK);
  gfx.commit();

  connectWifi();  // Here's the initial call to connect to the WiFi network

  Serial.println("Initializing touch screen...");
  ts.begin();

  Serial.println("Mounting file system...");
  bool isFSMounted = SPIFFS.begin();
  if (!isFSMounted) {
    Serial.println("Formatting file system...");
    drawProgress(50, "Formatting file system");
    SPIFFS.format();
  }
  drawProgress(100, "Formatting done");
  boolean isCalibrationAvailable = touchController.loadCalibration();
  if (!isCalibrationAvailable) {
    Serial.println("Calibration not available");
    touchController.startCalibration(&calibration);
    while (!touchController.isCalibrationFinished()) {
      gfx.fillBuffer(0);
      gfx.setColor(MINI_YELLOW);
      gfx.setTextAlignment(TEXT_ALIGN_CENTER);
      gfx.drawString(120, 160, "Please calibrate\ntouch screen by\ntouch point");
      touchController.continueCalibration();
      gfx.commit();
      yield();
    }
    touchController.saveCalibration();
  }

  timerPress = millis();  // Start sleep timer
  canBtnPress = true;

  connectHost();  // code to initialize connection to Host

  Serial.println("Startup complete");
}  // End of initial setup

int lastScreen;
unsigned long onAirTime;
long lastDrew = 0;
bool btnClick;
uint8_t MAX_TOUCHPOINTS = 10;
TS_Point points[10];
uint8_t currentTouchPoint = 0;

int screenCount = 6;  // how many different screens do we have in rotation starting at 0

void loop() { // Main Loop starts here ==================================================================================================================================

  ArduinoOTA.handle();

  if ((retString == "##CN0") || (retString == "##ID0"))  {
    connectHost();  // code to initialize connection to Host
  }

  // Repeated message to keep host connection alive every 5 seconds (host drops after 10 seconds) - Message is busy (RX/TX) query
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      writeHost("BY;");  // query if busy tranmitting
      readHost();

      if (screen < 9 ) {
        lastScreen = screen; // hold the last standard screen here
        onAirTime = millis();  // Reset the time for th eonAirTimer if not transmitting
      }

      if (retString == "BY0") {  // Must be transmitting
        screen = 9; // Screen 9 reserved for drawOnAir
      }
      else {
        screen = lastScreen;
      }
      return;
    }

    switch (screen)  { // Setup up a case select based on the current screen number (default is screen 0)
      case 0:
        drawMenu0();
        break;
      case 1:
        drawMenu1();
        drawWifiQuality();
        break;
      case 2:
        drawMenu2();
        drawWifiQuality();
        break;
      case 3:
        drawMenu3();
        drawWifiQuality();
        break;
      case 4:
        drawMenu4();
        drawWifiQuality();
        break;
      case 5:
        drawMenu5();
        drawWifiQuality();
        break;
      /*
            case 6:
              drawMenu6();
              drawWifiQuality();
              break;
            case 7:
              drawMenu7();
              drawWifiQuality();
              break;
            case 8:
              drawMenu8();
              drawWifiQuality();
              break;
      */
      case 9:
        drawOnAir();
        break;

      default:
        drawMenu0();
        break;
    }

    gfx.commit();

    if (SLEEP_INTERVAL_SECS && millis() - timerPress >= SLEEP_INTERVAL_SECS * 1000) {               // after 2 minutes go to sleep
      if (sleepTimerExp == false) {
        Serial.println(String(sleepTimerExp) + " Now entering sleep mode. Backlight off - touch screen to wake up ");
      
      digitalWrite(TFT_LED, LOW);    // HIGH to Turn on;
      sleepTimerExp = true;
      }
    }
    else {
      digitalWrite(TFT_LED, HIGH);
      sleepTimerExp = false;
    }
  }
}  // End of main loop ====================================================================================================================================

void drawOnAir()  {  // Screen to pop up when transmitting...

  gfx.fillBuffer(MINI_BLACK);

  if ((millis() - onAirTime) > 60000) { // 10 minute trasnmit timer turns ON AIR SCREEN full red
    gfx.fillBuffer(MINI_RED);
    gfx.drawString(120, 180, "ON AIR TIMER EXCEEDED");
  }

  gfx.setColor(MINI_DARKRED);
  gfx.fillRect(40, 60, 160, 60); //x, y, width, height

  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 80, "ON AIR");

}




void drawMenu0() { // Default screen is 8 Kenwood Host macro buttons-----------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);

  String MenuMsg1 = ""; // Clears top menu messages from any previous screen
  String MenuMsg2 = "";
  String MenuMsg = "";

  if (closeHost) {
    MenuMsg = ">> Host connection unavailable << ";
  }
  else {
    MenuMsg1 = String(Host_USER);
    MenuMsg2 = Host_IP;
    MenuMsg = MenuMsg2 + "   " + MenuMsg1;
  }

  // Draw Button routine = drawButton(int, String1, String2, String3) = int Button number, String1 top line, String2 next line, String 3 next time button 0 only)
  drawButton(0, "WD5ACP_WiFiPad", " - tap here for additional menus -", MenuMsg);       // This is the top menu button
  drawButton(1, "", "", "");
  drawButton(2, "", "", "");
  drawButton(3, "PB3", "Mute ON", "");
  drawButton(4, "PB4", "Mute OFF", "");
  drawButton(5, "RIT ON", "RIT OFF", "");
  drawButton(6, "", " ", "");
  drawButton(7, "PB7", "RIT < -5 Hz", "");
  drawButton(8, "PB8", "RIT > +5 Hz", "");

  buttNum = 99;
  readButton();
  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    delay(200);
  }
  if (buttNum == 1) {

  }
  if (buttNum == 2) {

  }
  if (buttNum == 3) {
    writeHost("MU1;"); // Mute ON
    readHost();
  }
  if (buttNum == 4) {
    writeHost("MU0;");  //Mute Off
    readHost();
  }
  if (buttNum == 5) {
    writeHost("RT;"); // Read RIT
    readHost();
    if (retString == "RT0") {  // If RIT on, turn it OFF
      writeHost("RT1;");
    }
    else {
      writeHost("RT0;");  // if RIT off, turn it ON
    }
    readHost();
  }
  if (buttNum == 6) {

  }
  if (buttNum == 7 ) {
    writeHost("RD00005;");  // Decrease RIT 5 hz
    readHost();
  }
  if (buttNum == 8) {
    writeHost("RU00005;");  // Increase RIT 5 hz
    readHost();
  }
}


void drawMenu1() { // Kenwood Host macro button screen for memory channels---------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);

  String MenuMsg1 = "";
  String MenuMsg2 = "";
  String MenuMsg = "";

  if (closeHost) {
    MenuMsg = ">> Host connection unavailable << ";
  }
  else {
    MenuMsg1 = String(Host_USER);
    MenuMsg2 = Host_IP;
    MenuMsg = MenuMsg2 + "   " + MenuMsg1;
  }
  drawButton(0, "WD5ACP_WiFiPad", " Memory Channel Menu", MenuMsg);       // This is the top menu button
  drawButton(1, "Mem CH 51", "> VFO A", "");
  drawButton(2, "Mem CH 52", "> VFO A", "");
  drawButton(3, "Mem CH 53", "> VFO A", "");
  drawButton(4, "Mem CH 54", "> VFO A", "");
  drawButton(5, "Mem CH 55", "> VFO A", "");
  drawButton(6, "", "", "");
  drawButton(7, "", "", "");
  drawButton(8, "", "", "");

  buttNum = 99;
  readButton();
  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    delay(200);
  }
  if (buttNum == 1) {
    writeHost("MN051;");  // Select Memory Channel
    readHost();
    writeHost("SV;"); // Host M/V mode command to VFO (required for tuner)
    readHost();
  }
  if (buttNum == 2) {
    writeHost("MN052;");  // Select Memory Channel
    readHost();
    writeHost("SV;"); // Host M/V mode command to VFO (required for tuner)
    readHost();
  }
  if (buttNum == 3) {
    writeHost("MN053;");  // Select Memory Channel
    readHost();
    writeHost("SV;"); // Host M/V mode command to VFO (required for tuner)
    readHost();
  }
  if (buttNum == 4) {
    writeHost("MN054;");  // Select Memory Channel
    readHost();
    writeHost("SV;"); // Host M/V mode command to VFO (required for tuner)
    readHost();
  }
  if (buttNum == 5) {
    writeHost("MN055;");  // Select Memory Channel
    readHost();
    writeHost("SV;"); // Host M/V mode command to VFO (required for tuner)
    readHost();
  }
  if (buttNum == 6) {
  }
  if (buttNum == 7) {
  }
  if (buttNum == 8) {
  }

}

void drawMenu2() { // Kenwood Host macro button screen for voice messages-----------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);

  String MenuMsg1 = "";
  String MenuMsg2 = "";
  String MenuMsg = "";

  if (closeHost) {
    MenuMsg = ">> Host connection unavailable << ";
  }
  else {
    MenuMsg1 = String(Host_USER);
    MenuMsg2 = Host_IP;
    MenuMsg = MenuMsg2 + "   " + MenuMsg1;
  }
  drawButton(0, "WD5ACP_WiFiPad", " Voice Message Menu", MenuMsg);       // This is the top menu button
  drawButton(1, "Voice Msg 1", "CQ CQ CQ", "");
  drawButton(2, "Voice Meg 2", "-", "");
  drawButton(3, "Voice Msg 3", "-", "");
  drawButton(4, "Voice Msg 4", "-", "");
  drawButton(5, "Voice Msg 5", "-", "");
  drawButton(6, "Voice Msg 6", "TEST", "");
  drawButton(7, "-", "-", "");
  drawButton(8, "Clear/Stop", "Voice Msgs", "");

  buttNum = 99;
  readButton();
  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    Serial.println("Screen switch selected from Menu 0");
    delay(200);
  }
  if (buttNum == 1) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB115;");  // Place back voice message 1
    readHost();
  }
  if (buttNum == 2) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB125;");  // Place back voice message 2
    readHost();
  }
  if (buttNum == 3) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB135;");  // Place back voice message 3
    readHost();
  }
  if (buttNum == 4) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB145;");  // Place back voice message 4
    readHost();
  }
  if (buttNum == 5) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB155;");  // Place back voice message 5
    readHost();
  }
  if (buttNum == 6) {
    writeHost("PB01;"); // open up voice message display
    readHost();
    writeHost("PB165;");  // Place back voice message 6
    readHost();
  }
  //if (buttNum == 7 ) {
  //  writeHost("RD00005;");
  // readHost();
  //}
  if (buttNum == 8) {
    writeHost("PB00;"); // close voice message list
    readHost();
  }
}

void drawMenu3() { //  Host Connection Menu -----------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);
  String MenuMsg1 = "";
  String MenuMsg2 = "";
  String MenuMsg = "";

  if (closeHost) {
    MenuMsg = ">> Host connection unavailable << ";
  }
  else {
    MenuMsg1 = String(Host_USER);
    MenuMsg2 = Host_IP;
    MenuMsg = MenuMsg2 + "   " + MenuMsg1;
  }
  drawButton(0, "WD5ACP_WiFiPad", "HOST Connection Menu", MenuMsg);       // This is the top menu button
  drawButton(7, "HOST IP", "CONNECT", "");
  drawButton(8, "HOST IP", "DISCONNECT", "");

  drawLabelValue(3, "Last data received:", retString);

  drawLabelValue(5, "Host IP address:", Host_IP);
  drawLabelValue(6, "Host IP port:", String(Host_PORT));
  drawLabelValue(7, "Host User Name:", Host_USER);
  drawLabelValue(8, "Host Password:", Host_PASS);
  drawLabelValue(9, "Host ID (if used):", Host_ID);
  drawLabelValue(10, "Host Login Status", loginStatus);

  buttNum = 99;
  readButton();
  if (buttNum == 7) {
    connectHost();
  }
  if (buttNum == 8) {
    Serial.println("closing Rig");
    closeHost = true;
    client.flush();
    client.stop();
  }
  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    delay(200);
  }
}

void drawMenu4() {  //----------------------------------------------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);
  drawButton(0, "WD5ACP_WiFiPad", " WiFi Connection Menu", "");       // This is the top menu button
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);

  drawLabelValue(2, "WiFi Status:", String(WiFi.status()));
  drawLabelValue(3, "WiFi ID:", String(WIFI_SSID));
  drawLabelValue(4, "WiFi Password:", String(WIFI_PASS));
  drawLabelValue(6, "WiFi Strength:", String(WiFi.RSSI()) + "dB");

  drawButton(7, "WiFi Setup", "from USB", "");

  buttNum = 99;
  readButton();

  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    delay(200);
  }
  if (buttNum == 7) {  // This is NOT functional at this time
    WiFiSetup();
  }
}

void drawMenu5() {  //----------------------------------------------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);

  drawButton(0, "WD5ACP_WiFiPad", " Application Information Menu", "");       // This is the top menu button

  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  drawLabelValue(2, "Version Number: ", String(VER));
  drawLabelValue(3, "Author:", "Guy W.");
  drawLabelValue(4, "", "Zebrick");

  drawLabelValue(6, "WiFi ID:", String(WIFI_SSID));
  drawLabelValue(7, "WiFi Strength:", String(WiFi.RSSI()) + "dB");
  drawLabelValue(9, "Heap Mem:", String(ESP.getFreeHeap() / 1024) + "kb");
  drawLabelValue(10, "Flash Mem:", String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
  drawLabelValue(11, "Chip ID:", String(ESP.getChipId()));
  drawLabelValue(12, "VCC: ", String(ESP.getVcc() / 1024.0) + "V");
  drawLabelValue(13, "CPU Freq.: ", String(ESP.getCpuFreqMHz()) + "MHz");
  char time_str[15];
  const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
  const uint32_t millis_in_hour = 1000 * 60 * 60;
  const uint32_t millis_in_minute = 1000 * 60;
  uint8_t days = millis() / (millis_in_day);
  uint8_t hours = (millis() - (days * millis_in_day)) / millis_in_hour;
  uint8_t minutes = (millis() - (days * millis_in_day) - (hours * millis_in_hour)) / millis_in_minute;
  sprintf(time_str, "%2dd%2dh%2dm", days, hours, minutes);
  drawLabelValue(14, "Uptime: ", time_str);

  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setColor(MINI_YELLOW);
  drawLabelValue(17, "Last Reset Cause:", String() );
  gfx.setColor(MINI_RED);
  gfx.drawStringMaxWidth(2, 300, 240 - 2 * 2, ESP.getResetInfo());

  buttNum = 99;
  readButton();
  if (buttNum == 0) {
    screen = (screen + 1) % screenCount;
    delay(200);
  }
}



void drawButton(int Num, String Lab1, String Lab2, String Lab3) {  // Subroutine to draw buttons on screen, Lab3 only used with menu button 0 -------------------------------------

  if (Num == 0) {
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(0, 0, 239, 55);
    gfx.setTransparentColor(MINI_BLACK);
    gfx.setColor(MINI_YELLOW);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(2, 0, Lab1);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    drawWifiQuality();
    gfx.setColor(MINI_YELLOW);
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(120, 18, Lab2 );
    gfx.setColor(MINI_WHITE);  // Used only with top button zero
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(120, 35, Lab3 );
  }
  if (Num == 1) {
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(0, 70, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(2, 74, Lab1 );
    gfx.drawString(2, 94, Lab2 );
  }
  if (Num == 2) {
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(129, 70, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(238, 74, Lab1 );
    gfx.drawString(238, 94, Lab2 );
  }
  if (Num == 3) {
    // Left Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(0, 135, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(2, 139, Lab1 );
    gfx.drawString(2, 159, Lab2);
  }
  if (Num == 4) {
    // Hostht Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(129, 135, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(238, 139, Lab1 );
    gfx.drawString(238, 159, Lab2 );
  }
  if (Num == 5) {
    // Left Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(0, 200, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(2, 204, Lab1 );
    gfx.drawString(2, 224, Lab2 );
  }
  if (Num == 6) {
    // Hostht Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(129, 200, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(238, 204, Lab1 );
    gfx.drawString(238, 224, Lab2);
  }
  if (Num == 7) {
    // Left Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(0, 265, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(2, 269, Lab1 );
    gfx.drawString(2, 289, Lab2 );
  }
  if (Num == 8) {
    // Hostht Button
    gfx.setColor(MINI_BLUE);
    gfx.drawRect(129, 265, 110, 45); //x, y, width, height
    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
    gfx.drawString(238, 269, Lab1 );
    gfx.drawString(238, 289, Lab2 );
  }
}

void readButton() { // This subroutine is for reading the touch pad button locations ------------------------------------------------------------------------------------------------------

  if (touchController.isTouched(0)) {
    timerPress = millis();  // Start sleep timer
    if (sleepTimerExp == false) {  // Only continue if not sleeping, otherwise don't act on the touch for a wakeup call

      buttNum = 99;
      TS_Point p = touchController.getPoint();

      // Top menu bar
      if (p.y < 60) { // Touched, but not a designated button....
        buttNum = 0;
      }
      // Button 1
      if ((p.y > 60) & (p.y < 125) & (p.x > 120) & (p.x < 240)) {
        buttNum = 1;
      }
      // Button 2
      if ((p.y > 70) & (p.y < 135) & (p.x > 0) & (p.x < 120)) {
        buttNum = 2;
      }
      // Button 3
      if ((p.y > 135) & (p.y < 200) & (p.x > 120) & (p.x < 240)) {
        buttNum = 3;;
      }
      // Button 4
      if ((p.y > 135) & (p.y < 200) & (p.x > 0) & (p.x < 120)) {
        buttNum = 4;
      }
      // Button 5
      if ((p.y > 200) & (p.y < 265) & (p.x > 120) & (p.x < 240)) {
        buttNum = 5;
      }
      // Button 6
      if ((p.y > 200) & (p.y < 265) & (p.x > 0) & (p.x < 120)) {
        buttNum = 6;
      }
      // Button 7
      if ((p.y > 265) & (p.x > 120) & (p.x < 240)) {
        buttNum = 7;
      }
      // Button 8
      if ((p.y > 265) & (p.x > 0) & (p.x < 120)) {
        buttNum = 8;
      }
      Serial.println("Button touched:> " + String(buttNum) + " <  ");
    }
  }
}




// Subroutine to connect to Host over IP ----------------------------------------------------------------------------------------------
void connectHost() {

  String UserString;
  String UserPass;
  String TotalPass;

  int UserLen = String(Host_USER).length();
  int PassLen = String(Host_PASS).length();

  if (Host_ID == "") { // See if there's a pre-configured Host ID. If not, calculate a Kenwood TS890 style ID one based on user name and password...

    if (UserLen < 10) {
      UserString = "0" + String(UserLen);
    }
    else {
      UserString = String(UserLen);
    }

    if (PassLen < 10) {
      UserPass = "0" + String(PassLen);
    }
    else {
      UserPass = String(PassLen);
    }
    TotalPass = "##ID0" + UserString + UserPass + String(Host_USER) + String(Host_PASS);
  }
  else {
    TotalPass = (Host_ID);  // If found in settings file, just use this one, dont' calculate it from user name and password.
  }

  Serial.println ("Using Admin login Code: >>>>> "  + TotalPass);

  drawProgress(5, "Connecting to Host - standby");
  delay (5000);  // 5 second delay here - need to make sure host is disconnected before connecting...

  if (!client.connect(Host_IP, Host_PORT)) {
    Serial.println("Host client connection failed");
    delay(500);
    closeHost = true;
    return;
  }
  else {

    Serial.println();
    Serial.println("Client connection at startup delay active...");
    drawProgress(30, "Connecting to Host in 5 seconds");
    delay(5000); // another 5 second delay here - need to make sure host is disconnected before connectin
    Serial.println("Sending initial connection request");

    if (client.connect(Host_IP, Host_PORT)) {
      closeHost = false;
      Serial.println("Writing Connection Request to Host");
      drawProgress(40, "Connecting to Host");
      writeHost("##CN;");
      readHost();
      delay(500);
      drawProgress(80, "Logging in");
      writeHost( TotalPass + ";");
      readHost();
      Serial.println("Here's the response from the ID login: " + retString);
      if (retString == "##ID1") {
        loginStatus = "ID accepted";
      }
      else {
        loginStatus = "Login ID failed!";
      }
      Serial.println (loginStatus);
    }
  }

  drawProgress(80, "Loggin complete...");
  Serial.println("login complete");
  delay (10);
  screen = 0;
}


void readHost() { //  Read response from host --------------------------------------------------------------------------------------------------------------------------

  static byte ndx = 0;
  char endMarker = ';';
  char rc;
  newData = true; // clean read

  delay (30);

  if (!(closeHost) && (client.available())) {
    Serial.print("Client available for reading data. ");
  }
  else {
    Serial.println("Client unavailable, unable to read from host.");
  }

  while (!(closeHost) && (client.available()) && (newData == true)) {
    rc = static_cast<char>(client.read());
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = false; // data read is complete
    }
    delay(10);
    retString = String(receivedChars);
  }
  newData = false;
  Serial.println("Last data read: " + retString);
}


void writeHost(String putString) { // write command to host --------------------------------------------------------------------------------------------------------------------------


  // This will send a string to the server
  if (!(closeHost) & (client.connected())) {
    Serial.print("Writing: " + putString + " ");
    client.print(putString);
    delay(200);
  }
  else {
    Serial.println("Unable to write to host, client not connected");
  }
}



void drawProgress(uint8_t percentage, String text) { // Graphic display bar -----------------------------------------------------------------------------------------
  gfx.fillBuffer(MINI_BLACK);
  // gfx.drawPalettedBitmapFromPgm(20, 5, ThingPulseLogo);
  // gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(120, 90, "https://thingpulse.com");
  gfx.setColor(MINI_YELLOW);

  gfx.drawString(120, 146, text);
  gfx.setColor(MINI_WHITE);
  gfx.drawRect(10, 168, 240 - 20, 15);
  gfx.setColor(MINI_BLUE);
  gfx.fillRect(12, 170, 216 * percentage / 100, 11);
  gfx.commit();
}



void drawLabelValue(uint8_t line, String label, String value) { // Easy graphic didisplay line maker -----------------------------------------------------------------
  const uint8_t labelX = 2;
  const uint8_t valueX = 136;
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setColor(MINI_YELLOW);
  gfx.drawString(labelX, 30 + line * 15, label);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(valueX, 30 + line * 15, value);
}

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() { // -----------------------------------------------------------------------------------------
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) {
    return 0;
  } else if (dbm >= -50) {
    return 100;
  } else {
    return 2 * (dbm + 100);
  }
}

void drawWifiQuality() { // ---------------------------------------------------------------------------------------------
  int8_t quality = getWifiQuality();
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  gfx.drawString(220, 0, String(quality) + "%");
  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (quality > i * 25 || j == 0) {
        gfx.setPixel(226 + 2 * i, 14 - j);
      }
    }
  }
}



void calibrationCallback(int16_t x, int16_t y) { //-------------------------------------------------------------------------------------------
  gfx.setColor(1);
  gfx.fillCircle(x, y, 10);
}



void WiFiSetup() { // When called, uses serial port to enter WiFi configuration --------------------------------------------------------------
  Serial.println("WiFi configuration via USB future enhancement ...................................");
}

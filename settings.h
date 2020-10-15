// Setup -Modify as needed per QTH  WiFi and Host --------------------------------------------------------------------
#define WIFI_SSID "zhome"            
#define WIFI_PASS ""
#define WIFI_HOSTNAME "WD5ACP_WiFiPad"

// This is the configuration to match the Admin user on the host. Default is for Kenwood TS-890 ----------------------
#define Host_IP "192.168.1.89"  // This is the static/fixed/non-changing IP address of the Host. 
#define Host_PORT 60000  // TCP Port 60000 required for Kenwood TS-890
#define Host_USER "WD5ACP"
#define Host_PASS "WD5ACP"
#define Host_ID "" // Alternative to using user name and password. Must type complete ID string. This is ignored if not just ""

// Program display config and  timers - change as desired. Note all timers are in SECONDs -----------------------------
//                  "..............." Message is centered in display above "ON AIR" 
#define ON_AIR_CALL "WD5ACP"           // 15 chars max!
const int ON_AIR_LIMIT = 600; // 10 minute timer 600000 = 10 mins
const int KEEP_ALIVE_TIME = 4; // secs to tickle host to keep connection alive
const int SLEEP_INTERVAL_SECS = 7200;  // in seconds - turns off backlight - touch screen to wake up


 
// Pins for the ILI9341  DO NOT CHANGE -------------------------------------------------------------------------------
#define TFT_DC D2
#define TFT_CS D1
#define TFT_LED D8

#define HAVE_TOUCHPAD
#define TOUCH_CS D3
#define TOUCH_IRQ  D4

/***************************
 * End Settings
 **************************/

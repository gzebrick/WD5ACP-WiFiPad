// Setup
#define WIFI_SSID "zhome"
#define WIFI_PASS ""
#define WIFI_HOSTNAME "WD5ACP_TS890_TouchPad"

#define Host_IP "192.168.1.89"
#define Host_PORT 60000

// Depending upon programming requirements - modify code as needed to assure proper connection. Default is Kenwood TS890 Admin login
#define Host_USER "WD5ACP"
#define Host_PASS "WD5ACP"
#define Host_ID "" // Alternative to using user name and password. Must type complete ID string. Ignored if not just ""

// Pins for the ILI9341
#define TFT_DC D2
#define TFT_CS D1
#define TFT_LED D8

#define HAVE_TOUCHPAD
#define TOUCH_CS D3
#define TOUCH_IRQ  D4

const int SLEEP_INTERVAL_SECS = 600;        // Going to sleep after idle times, set 0 for insomnia 600000 = 100 hours


/***************************
 * End Settings
 **************************/

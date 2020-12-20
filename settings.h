// required MaxMenus setting. Please onfigure max number of active menus 0-4. 
// MaxMenus = 0 means ONLY the ON-AIR timer funciton is active
// MaxMenus 1-4 will display macro push buttons menus. Menu 0 is the default view
// EVERY MENU requires a correstponding ConfigMenu settings. Configure each custom menu by editing corresponding ConfigMenu.h files

const int MaxMenus = 3;  // Set this number to match the quantity of ConfigMenu.h files you configured. This controls the menu rotation 

// Program display config and  timers - change as desired. Note all timers are in SECONDs -----------------------------
//                  "..............." Message is centered in display above "ON AIR" 
#define ON_AIR_CALL "WD5ACP"           // 15 chars max!
const int ON_AIR_LIMIT = 600; // 10 minute timer 600 = 10 mins
const int KEEP_ALIVE_TIME = 2; // secs to tickle host to keep connection alive
const int SLEEP_INTERVAL_SECS = 10800;  // 10800in seconds - turns off backlight - touch screen to wake up




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

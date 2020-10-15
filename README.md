# WD5ACP-WiFiPad

Use at your own risk! 

Arduino code files for WiFiPad app for use with Kenwood TS890 radio. WiFi pad has multiple push button screens that rotate
when the top main menu button (Button 0) is pressed. There's also an automatic ON AIR display triggered but the RX ready light going out.
(note the ON AIR display will also turn ON when the RX is muted - I may change this in a future release)

Buttons can be programmed to send commands, including multiple commands in sequence (a macro) as well as read data from the 
radio and do conditional actions. Commands must match Kenwood PC commands as documented by Kenwood

Look at the releases >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Right now the configuration is split between the hard-coded program and the settings.h file. Edit the Settings file to match your QTH!
I hope to change to program to move ALL of the configuration into the settings files (probably add a menusettings file), but for now
the button config is in the code. 

May also one day try to add a Web server page to change settings via a browser, but that's in the future. 

I wrote the code so each screen/menu reuses the button locations and code - just need to set up each menu's displayed text 
and edit the underlying code. Should be easy to figure out from the examples I left loaded in the app, including mult-line commands 
and even a conditional to have one button function as RIT ON (when off) and RIT OFF (when ON) - 

Anyway feel free to play with it. In the spirit of Amateur Radio it's up for grabs. I'm not that experienced with Arduino 
so if you want to jazz it up and share your fixes please do! 

My goal wasn't to make the best touchpad, only get one going for the lowest possible cost. 
I reused some of the code from the sample weather station app that the seller provides.

The little Arduino WEMOS + display has enough power and memory for this app, and you can power it right off the front USB plug on the radio!

The kit was under $30 US purchased from Amazon and only required minimum soldering of the connecting pins, maybe 5 minutes of work.
ESP8266 WiFi Color Display Kit
Brand: ThingPulse

Enjoy! 73s

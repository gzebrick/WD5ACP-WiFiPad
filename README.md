# WD5ACP-WiFiPad
Arduino code files for WiFiPad app for use with Kenwood TS890 radio

Use at your own risk! 

Look at the releases >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Okay I have it posted on GitHub. Version 4.10 - seems stable. I'm going to take a break from the project, 
but I think I have a good start and it's useable if you already know how to program an Arduino as you need to open the source code
to configure the buttons, etc.

There's more that could be done, 
like the WiFi setting via Serial, the Menu & Button configurations via a WebServer page, and even loading up the WiFi programming link,

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

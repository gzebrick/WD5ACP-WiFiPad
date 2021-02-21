# WD5ACP-WiFiPad
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

CLICK ON THE RELEASES TO THE RIGHT TO GET THE LATEST RELEASE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
THE SAMPLE WEATHER APP SHOULD BE INSTALLED FIRST AS A TEST IF YOU'RE NEW TO THIS ARDUINO >>>>>>
YOUR CONFIGURATION FILES WILL BE DIFFERENT FOR SETTING YOUT WiFi AND YOUR BUTTONS >>>>>>>>>>


Use at your own risk! Probably buggy!

Arduino code files for WiFiPad app for use with Kenwood TS890 radio. WiFi pad has multiple push button screens that rotate
when the top main menu button (Button 0) is pressed. There's also an automatic ON AIR display triggered when the radio returns a PA voltage > 0.
 ON AIR display may include your call sign or custom message, and there's a built-in volt meter for the PA volts as reported by the radio. There's also a
10 minute TX timer that will turn the screen red and indicate the timer has exceeded.

Buttons on the menus can be programmed to send commands, including up to 4  multiple commands in sequence (a macro). 
It is even possible to build complicated macros based on conditionals (if this, do that) but that requires your own custom programming inside the program.

Commands must match Kenwood PC commands as documented by Kenwood (.pdf file included).

Look at the releases >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

All basic configuration settings have been moved to the .h files. These may be edited as plain text. 
The setting.h file sets up the behaivor of the main program including setting the number of menus in rotation
The WiFiPadConfig.h file is mostly for setting the WiFi and host IP settings including user names and passwords
The 4 ConfigMenu.h files define the 4 custom button screens. Each screen has a 2 line descriptor, and each button
has a 2-line label as well as up to 4 sequenced commands.

I know the code is a bit crude, with little use of for and while loops, so there's a lot of redundant coding that someone with more 
time and experience with Arduino could no doubt to better - but for me, it seems to work fine.

Although it does not support it, it may be possible to enhance the code to store the settings in a plain .txt files that could be
read off the non-volatile memory in the WiFi pad. This would allow 'on the fly' reconfiguration without need of any programming or downloading.
There's file space and the little ESP Arduino boad supports it, but moving the settings from .h files to txt files, and then
providing code to read/write to the files, and finally adding HTTP or FTP server functionality to edit/load/save/delete the files is not 
part of the scope of this project. Could be done, just would take a lot of work.

Right now intial programming as well as configuration setup and any changes WILL require using the Arduino programmer and a download.

Anyway feel free to play with it. In the spirit of Amateur Radio it's the code is available for no charge for NON COMMERCIAL USE ONLY. 
I'm not that experienced with Arduino so if you want to jazz it up and share your fixes please do! 

My goal wasn't to make the best touchpad, only get one going for the lowest possible cost. 
I started off using some of the code from the sample weather station app that the seller provides, as well as borrowing from some of the provided
example code. It isn't the prettiest code I've ever done.

The little Arduino WEMOS + display has enough power and memory for this app, and you can power it right off the front USB plug on the radio!

The kit was under $30 US purchased from Amazon and only required minimum soldering of the connecting pins, maybe 5 minutes of work.
ESP8266 WiFi Color Display Kit
Brand: ThingPulse


IF YOU ARE NEW TO ARDUINO I RECOMMEND FOLLOWING THE THINGPULSE INSTRUCTIONS AND MAKE SURE YOU CAN RUN THE SAMPLE WEATHER STATION APP FIRST BEFORE
LOADING THIS CODE. IF YOU CAN"T GET THE WEATHER STATION APP TO RUN, CHANCES ARE THIS CODE WON'T WORK EITHER!

Good Luck! Enjoy! 73s

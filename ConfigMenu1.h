// Button Label4s should NOT include the ';' end of line charachter
// but any valid Macro4 commands sent  out require it.
// Up to 4 Macro4 commands may be used per button - if NOT required, set unused Macro4 command to ""
//
// Example
// #define Label161 "RIT OFF"   // Button 6, Label4 1 (top Label4) about 12 char max
// #define Label162 " & CLEAR"  // Button 6, Label4 2 (bottom Label4) about 12 char max
// #define Macro161 "RT0;"      // Button 6, Macro4 command 1, must terminate with ';' char
// #define Macro162 "RC;"       // Button 6, Macro4 command 2, must terminate with ';' char
// #define Macro163 ""          // Button 6, Macro4 command 3, unused and ignored
// #define Macro164 ""          // Button 6, Macro4 command 4, unused and ignored
//

// Top  Menu Button
#define Label101 " WiFi PAD"
#define Label102 "QUICK ACCESS"

// Button 1
#define Label111 "SSTV 20 w"
#define Label112 "AMP ON"
#define Macro111 "FA00014230000;"
#define Macro112 "OM0D;" 			// USB Data mode
#define Macro113 "FL01;"		// IF Filter B
#define Macro114 "PC020;"		// Amp ON, Power limit ON, power to 20 watts

// Button2
#define Label121 ""
#define Label122 ""
#define Macro121 ""
#define Macro122 "" 		
#define Macro123 ""		
#define Macro124 ""		

// Button3
#define Label131 "MR SCAN"
#define Label132 "SCOPE OFF"
#define Macro131 "MV1;" // Memory mode
#define Macro132 "BS00;" // Bandscope Off
#define Macro133 "SC01;"  // Scan ON
#define Macro134 "SQ175;"  // Squelch about 75%

// Button4
#define Label141 "MR SCAN OFF"
#define Label142 "MR to VFO"
#define Macro141 "SC00;"  // SCann Off
#define Macro142 "SV;" // VFO mode
#define Macro143 "BS01;" // Bandscope back on
#define Macro144 "SQ000;" // Squelch back to zero

// Button5
#define Label151 "AMP OFF"
#define Label152 "MAX-Po OFF"
#define Macro151 "LA10;" // Amp OFF
#define Macro152 "LP20;"  // Power Limit OFF
#define Macro153 "PC100;"  // Power to 100 watts
#define Macro154 ""

// Button6
#define Label161 "AMP ON"
#define Label162 "MAX-Po ON"
#define Macro161 "LA11;" // AMP ON
#define Macro162 "LP21;" // Power Limit ON
#define Macro163 "PC035;"  // Power to 35 watts
#define Macro164 ""

// Button7
#define Label171 ""
#define Label172 ""
#define Macro171 ""
#define Macro172 ""
#define Macro173 ""
#define Macro174 ""

// Button8
#define Label181 "USB MODE"
#define Label182 "HRH 20 MTRs"
#define Macro181 "FA00014183000;"
#define Macro182 "OM02;"      // USB mode
#define Macro183 "FL00;"   // IF Filter A
#define Macro184 "PC035;"   // Power to 35 watts



/***************************
   End Settings
 **************************/

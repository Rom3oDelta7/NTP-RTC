/*
	TABS=3
	Display the current timne and date on the Bynk dashboard

   Copyright 2016 Rob Redford
   This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.

*/

#define BLYNK_PRINT Serial    							// Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <NTPRTC.h>											// https://github.com/Rom3oDelta7/NTP-RTC
#include <WorldTimezones.h>								// https://github.com/Rom3oDelta7/NTP-RTC

// ============================= Blynk ==================================================

char auth[] = "AUTH";										// scan the QR code and clone the demo project to get an authorization token
char ssid[] = "SSID";
char pass[] = "PASSWORD";

// Blynk virtual pins
#define ACTION_BUTTON	V0									// pushbutton
#define DATE_DISPLAY		V1									// Labeled Value, L
#define TIME_DISPLAY		V2									// Labeled Value, L

/*
 SETUP
*/
void setup ( void ) {
	Serial.begin(115200);									// always instantiate Serial for OTA messages
	Serial.println("Starting ...");
	
	Blynk.begin(auth, ssid, pass);
	while ( !Blynk.connect() ) {							// wait for connection
		Blynk.run();
	}

	Serial.println("Blynk CONNECTED\n");

	Blynk.virtualWrite(TIME_DISPLAY, " ");
	Blynk.virtualWrite(DATE_DISPLAY, " ");
	// initialize NTP source for RTC data
	NTP_UTC_Timezone(UTC_MST);
	NTP_Init();
}

/*
 Blynk dashboard action button
 Display the curent date & time when pressed
*/
BLYNK_WRITE ( ACTION_BUTTON ) {
	displayDateTime();
}

/*
 update Blynk dashboard with the current time and date
 time value comes from NTP 
*/
void displayDateTime ( void ) {
	char currentTime[9];
	time_t	timeNow = now();
	
	// use a constant time value to avoid corner cases where, for example, the hour changes between function calls
	sprintf(currentTime, "%02d:%02d:%02d", hour(timeNow), minute(timeNow), second(timeNow));
	String currentDate = String(year(timeNow)) + "." + month(timeNow) + "." + day(timeNow);

	Serial.print("Current time: ");
	Serial.print(currentTime);
	Serial.print(" ");
	Serial.print(currentDate);
	Serial.println();

	Blynk.virtualWrite(TIME_DISPLAY, currentTime);
	Blynk.virtualWrite(DATE_DISPLAY, currentDate);
}

/*
 LOOP
*/
void loop ( void ) {
	Blynk.run();
	yield();
}
/*
	TABS=3
	
	NTP shim for TimeLib.h for the ESP8266
	
	A very simple library to provide the NTP setup functions to seed the time for TimeLib.h functions
	(hour(), day(), etc.)
	While this library works in the general case, the original motivtion for this was a replacemtn for the
	sketchy RTC Widgit in Blynk (www.blynk.cc)
	
 
   Based on Paul Stoffregen's TimeNTP_ESP8266WiFi.ino example and Time.h library
	See http://www.pjrc.com/teensy/td_libs_Time.html
	
	Copyright 2016 Rob Redford
   This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.

 */

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


#define DEBUG					0 

#define NTP_PACKET_SIZE		48				// NTP time is in the first 48 bytes of message
#define NTP_TIMEOUT			1500
#define NTP_SYNC_INTERVAL	300
#define LOCAL_PORT			8888			// local port to listen for UDP packets

// NTP Server list
static const char *NTPServerName[] = {
	(char *)"us.pool.ntp.org",
	(char *)"time.nist.gov",
	(char *)"time-a.timefreq.bldrdoc.gov",
	(char *)"time-b.timefreq.bldrdoc.gov",
	(char *)"time-c.timefreq.bldrdoc.gov",
	NULL};

uint8_t	packetBuffer[NTP_PACKET_SIZE]; 				//buffer to hold incoming & outgoing packets
int		timezone = 0;

WiFiUDP NTP_UDP;

static time_t 	getNTPTime(void);
static void 	sendNTPpacket(IPAddress&);
static time_t 	probeNTPServer(const char *);

/*
 Initialize UDP stream to NTP server
*/
void NTP_Init ( void ) {
#if DEBUG > 0
	Serial.println("NTPInit ...");
	Serial.print("IP address ");
	Serial.println(WiFi.localIP());
	Serial.println("Starting NTP UDP");
#endif
	NTP_UDP.begin(LOCAL_PORT);
#if DEBUG > 0
	Serial.print("Local port: ");
	Serial.println(NTP_UDP.localPort());
	Serial.println("waiting for sync");
#endif
	setSyncProvider(getNTPTime);
	setSyncInterval(NTP_SYNC_INTERVAL);
}

/*
 User function to set local timezone
*/
void NTP_UTC_Timezone ( const int offset ) {
	timezone = offset;
}

/*
  root time function
*/
static time_t getNTPTime ( void ) {
	time_t response = 0;
	
	// try each server in turn
	for ( uint8_t i = 0; (NTPServerName[i] != NULL) && (response == 0); i++ ) {
#if DEBUG > 1
		Serial.printf("Trying NTP server %s\n", NTPServerName[i]);
#endif
		response = probeNTPServer(NTPServerName[i]);
	}
	return response;
}

/*
 try an individual NTP server
*/
static time_t probeNTPServer ( const char *server ) {
	IPAddress NTPServerIP; 												// NTP server's ip address

	while ( NTP_UDP.parsePacket() > 0 ) ; 							// discard any previously received packets
	
	// get a random server from the pool
	WiFi.hostByName(server, NTPServerIP);
#if DEBUG >= 2
	Serial.println("NTP Request");
	Serial.print(server);
	Serial.print(": ");
	Serial.println(NTPServerIP);
	Serial.printf("First octet: %d\n", NTPServerIP[0]);
#endif
	if ( NTPServerIP[0] != 0 ) {
		sendNTPpacket(NTPServerIP);
		uint32_t beginWait = millis();
		while ( (millis() - beginWait) < NTP_TIMEOUT ) {
			int size = NTP_UDP.parsePacket();
			if ( size >= NTP_PACKET_SIZE ) {
#if DEBUG >= 2
				Serial.println("Receive NTP Response");
#endif
				NTP_UDP.read(packetBuffer, NTP_PACKET_SIZE);  		// read packet into the buffer
				unsigned long secsSince1900;
				// convert four bytes starting at location 40 to a long integer
				secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
				secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
				secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
				secsSince1900 |= (unsigned long)packetBuffer[43];
				return secsSince1900 - 2208988800UL + timezone * SECS_PER_HOUR;
			}
		}
		// timeout
#if DEBUG > 0
		Serial.println("No NTP Response :-(");
#endif
		return 0;
	} else {
		// address not resolved
		return 0;
	}
}

/*
 consstruct and send an NTP request to the time server at the given IP address
*/
static void sendNTPpacket ( IPAddress &address ) {
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	packetBuffer[0] = 0b11100011;   				// LI, Version, Mode
	packetBuffer[1] = 0;     						// Stratum, or type of clock
	packetBuffer[2] = 6;     						// Polling Interval
	packetBuffer[3] = 0xEC;  						// Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// send a packet requesting a timestamp:
	NTP_UDP.beginPacket(address, 123); 			//NTP requests are to port 123
	NTP_UDP.write(packetBuffer, NTP_PACKET_SIZE);
	NTP_UDP.endPacket();
}

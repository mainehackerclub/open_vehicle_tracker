#include <Adafruit_CC3000.h>	// https://github.com/adafruit/Adafruit_CC3000_Library
#include <Adafruit_GPS.h>		// https://github.com/adafruit/Adafruit-GPS-Library
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"		// This should be in Adafruit_CC3000 library

#if ARDUINO >= 100
 #include <SoftwareSerial.h>
#else
  // Older Arduino IDE requires NewSoftSerial, download from:
  // http://arduiniana.org/libraries/newsoftserial/
// #include <NewSoftSerial.h>
 // DO NOT install NewSoftSerial if using Arduino 1.0 or later!
#endif

#if ARDUINO >= 100
  SoftwareSerial mySerial(8, 7);
#else
  NewSoftSerial mySerial(8, 7);
#endif
Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  true

//#define CC3000_TINY_DRIVER

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed

#define WLAN_SSID       "Alienseed_G" //"Maine Discovery Museum"           // cannot be longer than 32 characters!
#define WLAN_PASS       "" //"toomuchfun"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

const unsigned long connectTimeout  = 15L * 1000L; // Max time to wait for server connection

char serverName[] = "awesomesauce.me"; //"141.114.192.128";

// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!
#define GPSRATE 9600

// The buffer size that will hold a GPS sentence. They tend to be 80 characters long
// so 90 is plenty.
#define BUFFSIZ 75 // plenty big

// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char *parseptr;              // a character pointer for parsing
char buffidx;                // an indexer into the buffer

int truckid=-1;

uint32_t ip;

void setup(void)
{
  Serial.begin(115200);
  
  Serial.println(F("Hello OVT User!\n")); 
  InitializeWireless();
  InitializeGPS();
 
 Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

}

void InitializeWireless()
{
   displayDriverMode();

   /* Initialise the module */
  Serial.println(F("\nInitializing Wireless..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't initialize wireless! Check your wiring?"));
    while(1);
  }
  
  displayMACAddress();

  // Optional SSID scan
  // listSSIDResults();
  
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  ip = 0;
  // Try looking up the website's IP address
  Serial.print(serverName); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(serverName, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }

  cc3000.printIPdotsRev(ip);
    
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  //Serial.println(F("\n\nDisconnecting"));
  //cc3000.disconnect();
}

void displayDriverMode(void)
{
  #ifdef CC3000_TINY_DRIVER
    Serial.println(F("CC3000 is configure in 'Tiny' mode"));
  #else
    Serial.print(F("RX Buffer : "));
    Serial.print(CC3000_RX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
    Serial.print(F("TX Buffer : "));
    Serial.print(CC3000_TX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
  #endif
}

void displayMACAddress(void)
{
  uint8_t macAddress[6];
  
  if(!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}

void InitializeGPS()
{
  // 9600 NMEA is the default baud rate for MTK - some use 4800
  Serial.println("Initializing GPS");

  GPS.begin(GPSRATE);
  
  // You can adjust which sentences to have the module emit, below
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // uncomment this line to turn on only the "minimum recommended" data for high update rates!
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // uncomment this line to turn on all the available data - for 9600 baud you'll want 1 Hz rate
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  
  // Set the update rate
  // 1 Hz update rate
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // 5 Hz update rate- for 9600 baud you'll have to set the output to RMC or RMCGGA only (see above)
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  // 10 Hz update rate - for 9600 baud you'll have to set the output to RMC only (see above)
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
  
}

unsigned long last=0;

void loop(void)
{
    char* result;

	checkTruck();

	result = ProcessLine();
	if(result!=NULL)
	{
		if(millis() - last > 5000)
		{
			postToServer(result);
			last=millis();
		}
	}
}

void checkTruck()
{
    int selection = 1984;

    //if(!digitalRead(A0)) selection=2;
    //else if(!digitalRead(A1)) selection=3;
    //else if(!digitalRead(A2)) selection=4;
    //else if(!digitalRead(A3)) selection=5;
    //else if(!digitalRead(A4)) selection=6;
    //else if(!digitalRead(A5)) selection=7;

    if(selection!=truckid)
    {
        truckid=selection;
        printTruck();
    }
}

void printTruck()
{
	char truck[5] ;
    sprintf(truck,"%d",truckid);
    Serial.print("TruckID: ");
    Serial.println(truck);
}

char* ProcessLine()
{
	char Time[10];
	char Truck[5];
	char Longitude[12];
	char Latitude[12];

	uint32_t tmp;

    readline();

    if (strncmp(buffer, "$GPGGA",6) == 0) {
		Serial.println("Processing Message");

        parseptr = buffer+7;
        //$GPGGA,021928.585,4448.1422,N,06846.1962,W,6,00,50.0,15.7,M,-30.3,M,,0000*63

        tmp = parsedecimal(parseptr); 
        uint32_t hour = tmp / 10000;
        uint32_t minute = (tmp / 100) % 100;
        uint32_t second = tmp % 100;

        sprintf(Time,"%02lu:%02lu:%02lu",hour,minute,second);

        parseptr = strchr(parseptr, ',') + 1;

		sprintf(Truck,"%d",truckid);

        // latitude
        uint32_t lat1;
        uint32_t lat2;
        readDdmmAsDeg(&parseptr,lat1,lat2);

        parseptr = strchr(parseptr, ',') + 1;
        // read latitude N/S data
        char latdir;
        if (parseptr[0] != ',') {
            latdir = parseptr[0];
        }
        parseptr = strchr(parseptr, ',') + 1;

        const char* latsgn= latdir=='N' ? "" : "-";

        sprintf(Latitude,"%s%lu.%lu",latsgn,lat1,lat2);

        // longitude
        uint32_t lon1;
        uint32_t lon2;
        readDdmmAsDeg(&parseptr,lon1,lon2);

		if(lat1==0 && lat2==0 && lon1==0 && lat2==0)
		{
			Serial.println("GPS Location not available.");
			return NULL;
		}

        parseptr = strchr(parseptr, ',')+1;
        // read longitude E/W data
        char longdir;
        if (parseptr[0] != ',') {
            longdir = parseptr[0];
        }

        const char* lonsgn= longdir=='E' ? "" : "-";

        sprintf(Longitude,"%s%lu.%lu",lonsgn,lon1,lon2);

        // pos fix ind: dump it
        parseptr = strchr(parseptr, ',')+1;
        tmp = parsedecimal(parseptr); 

        parseptr = strchr(parseptr, ',')+1;
        uint32_t numsats = parsedecimal(parseptr); 
        //sprintf(Sats,"%lu",numsats);

		sprintf(buffer,"{\"Time\": \"%s\",\"Truck\": \"%s\",\"Lat\": \"%s\", \"Lon\": \"%s\"}",Time,Truck,Latitude,Longitude);
        Serial.println(buffer);
        return buffer;
    }
	return NULL;
}

void readDdmmAsDeg(char** parseptr, uint32_t &deg, uint32_t &dec)
{
    uint32_t first = parsedecimal(*parseptr);
    if (first != 0) {
        deg=first/100;
        dec = (first % 100) * 10000;
        *parseptr = strchr(*parseptr, '.')+1;
        dec += parsedecimal(*parseptr);
        dec = (dec * 10) / 6;
    }
    else
    {
        deg=0;
        dec=0;
    }
    return;
}

void postToServer(char* DataString){
  
 unsigned long startTime, t = 0L;

 char cLength[5]="";

 int stringLength = 0;
  while(1){
    if(DataString[stringLength] == 0){
      break;
    }
    stringLength++;
  }

  sprintf(cLength, "%u",stringLength);

  Serial.print("Posting ");
  Serial.print(stringLength);
  Serial.println( " bytes...");

  Serial.print("Connecting to ");
  cc3000.printIPdotsRev(ip);
  Serial.println("...");

  Adafruit_CC3000_Client client;
  startTime = millis();
    do {
      client = cc3000.connectTCP(ip,80); Serial.print('.');
    } while((!client.connected()) &&
		((millis() - startTime) < connectTimeout));

  if(client.connected()){ 
    Serial.println("Making HTTP POST");
    client.fastrprint(F("POST /post.php HTTP/1.0\r\n"));
    client.fastrprint(F("Host: "));
    client.fastrprint(serverName);
    client.fastrprint(F("\r\nContent-Type: application/json\r\n"));
    client.fastrprint(F("Content-Length: "));
    client.fastrprint(cLength);
    client.fastrprint(F("\r\n\r\n"));
    client.fastrprint(DataString);

	delay(1000);

    client.close();
  } else {
    Serial.println("Error Connecting");  
  }
}

uint32_t parsedecimal(char *str) {
    uint32_t d = 0;

    while (str[0] != 0) {
        if ((str[0] > '9') || (str[0] < '0'))
            return d;
        d *= 10;
        d += str[0] - '0';
        str++;
    }
    return d;
}

void readline(void) {
    char c;

    buffidx = 0; // start at begninning
    while (1) {
        c=GPS.read();
        OnChar(c);
		if (buffidx == 0 && c != '$')
            continue;
        else if (c == -1)
            continue;
        if (c == '\n')
            continue;
        if ((buffidx == BUFFSIZ-1) || (c == '\r')) {
            buffer[buffidx] = 0;
            return;
        }
        buffer[buffidx++]= c;
    }
}

void OnChar(char c)
{
	if(GPSECHO)
		Serial.print(c);
}



/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

// A simple sketch to read GPS data and parse the $GPRMC string 
// see http://www.ladyada.net/make/gpsshield for more info

// If using Arduino IDE prior to version 1.0,
// make sure to install newsoftserial from Mikal Hart
// http://arduiniana.org/libraries/NewSoftSerial/
#if ARDUINO >= 100
#include "Arduino.h"
#include "SoftwareSerial.h"
#else
#include "WProgram.h"
#include "NewSoftSerial.h"
#endif

#include <SPI.h>
#include <Ethernet.h>

// Use pins 2 and 3 to talk to the GPS. 2 is the TX pin, 3 is the RX pin
#if ARDUINO >= 100
SoftwareSerial mySerial = SoftwareSerial(2, 3);
#else
NewSoftSerial mySerial = NewSoftSerial(2, 3);
#endif

// Use pin 4 to control power to the GPS
#define powerpin 4

// Set the GPSRATE to the baud rate of the GPS module. Most are 4800
// but some are 38400 or other. Check the datasheet!
#define GPSRATE 4800
//#define GPSRATE 38400

// The buffer size that will hold a GPS sentence. They tend to be 80 characters long
// so 90 is plenty.
#define BUFFSIZ 90 // plenty big


// global variables
char buffer[BUFFSIZ];        // string buffer for the sentence
char *parseptr;              // a character pointer for parsing
char buffidx;                // an indexer into the buffer

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };

EthernetClient client;

char serverName[] = "141.114.192.128";

char Time[50];
char Truck[50];
char Longitude[50];
char Latitude[50];
char Sats[50];

int truckid=-1;

void setup() 
{ 
    if (powerpin) {
        pinMode(powerpin, OUTPUT);
    }

    // connect to the serial terminal at 9600 baud
    Serial.begin(9600);

    // connect to the GPS at the desired rate
    mySerial.begin(GPSRATE);

    // prints title with ending line break 
    Serial.println("GPS Telemetry Poster: Maine Hacker Club"); 
    Serial.println("Starting..."); 

    digitalWrite(powerpin, LOW);         // pull low to turn on!

    Serial.println("Initializing...");
    while(!Ethernet.begin(mac)){
        Serial.println("Could not begin Ethernet..");
        delay(1000);
    }

    pinMode(A0,INPUT_PULLUP);
    pinMode(A1,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
    pinMode(A3,INPUT_PULLUP);
    pinMode(A4,INPUT_PULLUP);
    pinMode(A5,INPUT_PULLUP);
} 


void loop() 
{ 
    checkTruck();

    ProcessLine();
}

void checkTruck()
{
    int selection = 1;

    if(!digitalRead(A0)) selection=2;
    else if(!digitalRead(A1)) selection=3;
    else if(!digitalRead(A2)) selection=4;
    else if(!digitalRead(A3)) selection=5;
    else if(!digitalRead(A4)) selection=6;
    else if(!digitalRead(A5)) selection=7;

    if(selection!=truckid)
    {
        truckid=selection;
        printTruck();
    }
}

void printTruck()
{
    sprintf(Truck,"%d",truckid);
    Serial.print("TruckID: ");
    Serial.println(Truck);
}

void ProcessLine()
{

    uint32_t tmp;

    readline();

    if (strncmp(buffer, "$GPGGA",6) == 0) {
        parseptr = buffer+7;
        //$GPGGA,021928.585,4448.1422,N,06846.1962,W,6,00,50.0,15.7,M,-30.3,M,,0000*63

        tmp = parsedecimal(parseptr); 
        uint32_t hour = tmp / 10000;
        uint32_t minute = (tmp / 100) % 100;
        uint32_t second = tmp % 100;

        sprintf(Time,"%02lu:%02lu:%02lu",hour,minute,second);

        parseptr = strchr(parseptr, ',') + 1;

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
        sprintf(Sats,"%lu",numsats);

        char* toPost=ConstructJson(Time,Truck,Latitude,Longitude,Sats);
        Serial.println(toPost);
        postToServer(toPost);
    }

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
  Serial.println("Posting...");
  int stringLength = 0;
  while(1){
    if(DataString[stringLength] == 0){
      break;
    }
    stringLength++;
  }
  Serial.println("Connecting...");
  if(client.connect(serverName,80)){ 
    Serial.println("Making HTTP Request");
    client.println("POST /post.php HTTP/1.0");
    client.print("Host: ");
    client.println(serverName);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(stringLength);
    client.println();
    client.println(DataString);

    client.stop();
  } else {
    Serial.println("Error Connecting");  
  }
}

char jsonbuffer[255];
char* ConstructJson(char* Time, char* Truck, char* Lat, char* Lon, char* Sats )
{
    char* format="{\"Time\": \"%s\",\"Truck\": \"%s\",\"Lat\": \"%s\", \"Lon\": \"%s\",  \"Sats\": \"%s\"}";

    sprintf(jsonbuffer,format,Time,Truck,Lat,Lon,Sats);

    return jsonbuffer;
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
        c=mySerial.read();
        if (c == -1)
            continue;
        OnChar(c);
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
    //Serial.print(c);
}


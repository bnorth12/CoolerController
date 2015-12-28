/*
 * Project borrows heavily from the ESP8266 Captive Portal Arduino ESP8266 Example.
 *
 * Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config.
 * If it does not then navigate to http://192.168.4.1/wifi and config it there.
 * Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.
 *
 * Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.
 *
 * MQTT is currently publishing data to io.AdaFruit
 *
 */


//
/**********************************************************************************************************************/
/*
 * TODO: Test Heater Logic
 * TODO: Store and retrieve values for to/from eeprom, 1 byte each
 * TODO: Find best values for heater PWM for heater On and Heater Off 'Standby'
 */
/**********************************************************************************************************************/



#define DEBUG

#ifdef DEBUG  //Sends data to Serial for debugging
#define debug(x)     Serial.print(x);
#define debugln(x)   Serial.println(x);
#else
#define debug(x)     // define empty, so macro does nothing
#define debugln(x)
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi parameters
#define WLAN_SSID       "your ssid"
#define WLAN_PASS       "you network password"

// Set Pin Number for one wire bus
#define ONE_WIRE_BUS_PIN 12    // GPIO12 

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "your user name"
#define AIO_KEY         "your AIO Key"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;

// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);


//****************************** Feeds ***************************************
// Create and Correct feeds for Cooler Controller
const char LIGHTMETER_FEED[] PROGMEM = AIO_USERNAME "/feeds/lightmeter";
Adafruit_MQTT_Publish lightmeter = Adafruit_MQTT_Publish(&mqtt, LIGHTMETER_FEED);

// Setup feeds for heatertemp htrTemp
 const char HEATERTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/heatertemp";
 Adafruit_MQTT_Publish heatertemp = Adafruit_MQTT_Publish(&mqtt, HEATERTEMP_FEED);

// Setup feeds for heatersettemp   htrTmpSetPT
 const char HEATERSETTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/heatersettemp";
 Adafruit_MQTT_Publish heatersettemp = Adafruit_MQTT_Publish(&mqtt, HEATERSETTEMP_FEED);

// Setup feeds for heaterpwm  htrPWM
 const char HEATERPWM_FEED[] PROGMEM = AIO_USERNAME "/feeds/heaterpwm";
 Adafruit_MQTT_Publish heaterpwm = Adafruit_MQTT_Publish(&mqtt, HEATERPWM_FEED);

// Setup feeds for heatermaxtemp atMaxHtrTmp
 const char HEATERMAXTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/heatermaxtemp";
 Adafruit_MQTT_Publish heatermaxtemp = Adafruit_MQTT_Publish(&mqtt, HEATERMAXTEMP_FEED);

// Setup feeds for fintemp   finTemp
 const char FINTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/fintemp";
 Adafruit_MQTT_Publish fintemp = Adafruit_MQTT_Publish(&mqtt, FINTEMP_FEED);

// Setup feeds for finsettemp fimTmpSetPt
 const char FINSETTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/finsettemp";
 Adafruit_MQTT_Publish finsettemp = Adafruit_MQTT_Publish(&mqtt, FINSETTEMP_FEED);

// Setup feeds for defrostcycle defrostCycleOn
 const char DEFROSTCYCLE_FEED[] PROGMEM = AIO_USERNAME "/feeds/defrostcycle";
 Adafruit_MQTT_Publish defrostcycle = Adafruit_MQTT_Publish(&mqtt, DEFROSTCYCLE_FEED);

// Setup feeds for coolertemp rmTemp
 const char COOLERTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/coolertemp";
 Adafruit_MQTT_Publish coolertemp = Adafruit_MQTT_Publish(&mqtt, COOLERTEMP_FEED);

// Setup feeds for coolersetpoint rmTmpSetPt
 const char COOLERSETPOINT_FEED[] PROGMEM = AIO_USERNAME "/feeds/coolersetpoint";
 Adafruit_MQTT_Publish coolersetpoint = Adafruit_MQTT_Publish(&mqtt, COOLERSETPOINT_FEED);

// Setup feeds for coolcycle coolCycleOn
 const char COOLCYCLE_FEED[] PROGMEM = AIO_USERNAME "/feeds/coolcycle";
 Adafruit_MQTT_Publish coolcycle = Adafruit_MQTT_Publish(&mqtt, COOLCYCLE_FEED);


/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass address of our oneWire instance to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// TODO: set probe values for project these came from differant probes
// Set the Prove Addresses
DeviceAddress rmProbe = { 0x28, 0x2E, 0x3D, 0x1D, 0x07, 0x00, 0x00, 0x0C }; // Room Temp Probe
DeviceAddress finProbe = { 0x28, 0x99, 0x21, 0x1E, 0x07, 0x00, 0x00, 0x5B }; // Fin Temp Probe
DeviceAddress htrProbe = { 0x28, 0x67, 0x47, 0x1E, 0x07, 0x00, 0x00, 0xB7 }; // Heater Temp Probe



/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "ESP_ap";
const char *softAP_password = "12345678";

/* hostname for mDNS. Should work at least on windows. Try http://esp8266.local */
const char *myHostname = "esp8266";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[32] = "";
char password[32] = "";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;

/** Current WLAN status */
int status = WL_IDLE_STATUS;


// Global Variables TODO: eliminate  as many Globals as possible. Initial Coding is a Proof on Conecept implementation

// TODO: Change pin assignments to #define rather than declaration
byte wirePin = 4;  // Pin used for Sensors
//#define wirePin #
byte  htrPin1 = 5; // Pin Connected to Heater1
byte  htrPin2 = 4; // Pin Connected to Heater2
//#define doorPin #
byte doorPin = 10; // input used with door sensor
byte htrPWM = 128; //Pulse Width Modulation Value for setting Heater Temperature Inital vale ~50% duty Factor
byte htrPMWhigh = 128; //
byte htrPMWlow = 10; //
byte rmTemp = 80; // Room Temperature
byte finTemp = 35; // Fin Temperature
byte htrTemp = 72; // Heater Temperature
byte rmTmpSetPt = 34; // Set Point for Room
byte htrTmpSetPT = 85; // Set Point for Heater Value
byte fimTmpSetPt = 0; //Ice Detetion Temperature Set Point
byte rmDeadBand = 4; // Dead Band for room temp difference between low and high allowed temperatures
byte finDeadBand = 3; // Dead Band for determining heater operation
byte htrDeadBand = 3; // Dead band for Heater operation keep the heater close to AC on/off setting
boolean atMaxHtrTmp = false; // Boolean for heater temp control
boolean coolCycleOn = false; // cool cycle is active, can be overridden by defrost cycle
boolean defrostCycleOn = false; // defrost cycle on to prevent icing
boolean htrOn = false; // heater on or off, controlled by coolCycleOn and defrostCycleOn Varibles


/**********************************************************************************************************************/
//Variables for no delay processing TODO: eliminate globals
/**********************************************************************************************************************/
const int rg1 = 1000;
//const int rgFiveMin = 300000;
const int rgFiveMin = 30000;
int previousRG1 = rg1;
int previousRGFiveMin = rgFiveMin;

/**********************************************************************************************************************/
//Setup Function
/**********************************************************************************************************************/
void setup() {
  // put your setup code here, to run once:

  delay(1000);
  
#ifdef DEBUG
  Serial.begin(115200);

  debugln("Starting Setup");
  debugln();
  debug("Configuring access point...");
#endif

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  debug("AP IP address: ");
  debugln(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/para", handleParameters);
  server.on("/parasave", handleParaSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound ( handleNotFound );
  server.begin(); // Web server start
  debugln("HTTP server started");
  loadCredentials(); // Load WLAN credentials from network
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID


  // This is setup for the cooler controller
  //------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(rmProbe, 10);
  sensors.setResolution(finProbe, 10);
  sensors.setResolution(htrProbe, 10);

  // Set Heater Pin as output
  pinMode(htrPin1, OUTPUT);
  pinMode(htrPin2, OUTPUT);

  loadSettings(); // Load the Settings for the Cooler from EEPROM
  if (fimTmpSetPt == 0) {
    // Initial Settings not Set Load Defaults
    rmTmpSetPt = 34;
    fimTmpSetPt = 32;
    htrTmpSetPT = 85;
    rmDeadBand = 4;
    finDeadBand = 3;
    htrDeadBand = 3;

  } // End of loading Cooler Settings


} // end setup

/**********************************************************************************************************************/
//WiFi Connect for Station Mode
/**********************************************************************************************************************/
void connectWifi() {
  debugln("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int connRes = WiFi.waitForConnectResult();
  debug( "connRes: " );
  debugln ( connRes );
} // end connect WiFi


/**********************************************************************************************************************/
/*
*Loop Function
*Web Page Section
*
*Cooler Section
* Request Temps Function Call
* Set the Temp Variables to the values retrieved from Temp Probes
*/
/**********************************************************************************************************************/
void loop() {
  // put your main code here, to run repeatedly:

  //  requestTemps(); //TODO: call at 1Hz
  //  delay(10); // delay function on esp8266 library allows OS to run background tasks
  //  setTemps(); //TODO: call at 1Hz // commented out with no hardware connected
  //  delay(10); // delay function on esp8266 library allows OS to run background tasks
  //  setHeaterState(); // Set use temps, set points, current state, previous state to new heater state



  if (connect) {
    debugln ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s) { // WLAN status change
      debug( "Status: " );
      debugln( s );
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        debugln( "" );
        debug( "Connected to " );
        debugln( ssid );
        debug( "IP address: " );
        debugln( WiFi.localIP() );

        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          debugln("Error setting up MDNS responder!");
        } else {
          debugln("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
  }
  // Do work:

  // Loop functions for Cooler Controller
  timing();
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();



} // end loop




#include <vector>
#include <set>
#include <tinyxml2.h>
#include <Adafruit_DotStar.h> // Because conditional #includes don't work w/Arduino sketches...
#include <ESP8266WiFi.h>

using namespace tinyxml2;
using namespace std;

#define NUMPIXELS  24 // Number of LEDs in strip
#define DATAPIN    12
#define CLOCKPIN   13
#define BUTTONPIN  5 // Pin number of button
#define BUTTONPIN2 5 // another button


Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

// Mac Address: 5C:CF:7F:1A:03:57
// Connect to GTWIFI: https://auth.lawn.gatech.edu/index.php


// Deine Vehicle class with it's nearest bus
class Vehicle
{
    public:
    String vehicleID;
    String stopTag;
    String arrTime;
    String direction;
};


// GT WIFI
const char* ssid = "GTother";
const char* password = "GeorgeP@1927";
const char* host = "gtbuses.herokuapp.com";

/*
* --------------------------------
* Route information * Direction
* --------------------------------
*/
String RED_ROUTE_DIRECTION[2] = {
    "Fitten Hall",
    "North Ave Apts"
};


String BLUE_ROUTE_DIRECTION[2] = {
    "Fitten Hall",
    "North Ave Apts"
};

String TROLLEY_ROUTE_DIRECTION[2] = {
    "To MARTA Station",
    "Transit Hub"
};

uint32_t RED_COLOR = 0xFF0000;      // color for red
uint32_t BLUE_COLOR = 0x0000FF;     // color for blue
uint32_t ORANGE_COLOR = 0xFFA500;   // color for trolley

#define NUM_BUSSTOPS 24

// Define light index
#define TROLLEY_D1_TO_MARTA 11
#define TROLLEY_D2_TRANSIT_HUB 23

// Trolley stops
String STOP2INDEX[NUM_BUSSTOPS] = {
    "marta_a",
    "wpe7mrt",
    "duprmrt",
    "techsqua_ib",
    "tech5mrt",
    "fersfomrt",
    "ferschmrt",
    "fersatl_ib",
    "fershemrt",
    "ferstdr",
    "centrstud",
    "", // direction light
    "tranhub_a",
    "studcentr",
    "reccter",
    "fersherec",
    "fersatla",
    "ferschrec",
    "fersforec",
    "tech5rec",
    "techsqua",
    "",
    "",
    "" // direciton light
};


/*
* --------------------------------
* Global variables to tracking:
* - route
* - direction
* - running VehiclesIDs
* - color of the car
* --------------------------------
*/
// Vehicle array
String route;
String direction;
set <String> runningVehicleIDs;
int numOfVehicles = 0;
Vehicle vehicles[10];
int prevLightTimeStamp [10];
bool busLight[NUM_BUSSTOPS];
uint32_t color = 0;

WiFiClientSecure client;
XMLDocument doc;


/*
* --------------------------------
* getRunningVehicles()
* 
* Given route & direction, check how many buses are running
*
* Set variables: 
* - numOfVehicles 
* - runningVehicleIDs
* --------------------------------
*/
void getRunningVehicles(XMLElement * predictionsNode)
{
    String vid;    
    vid = predictionsNode->Attribute("agencyTitle");
    Serial.println(vid);
    
    XMLElement * directionNode = predictionsNode->FirstChildElement("direction");
    XMLElement * predictionNode = directionNode->FirstChildElement("prediction");

    vid = predictionNode->Attribute("vehicle");
    if( runningVehicleIDs.find(vid) == runningVehicleIDs.end())
    {
        runningVehicleIDs.insert(vid);
    }

    while(predictionNode->NextSiblingElement("prediction") != NULL)
    {
        predictionNode = predictionNode->NextSiblingElement("prediction");
        vid = predictionNode->Attribute("vehicle");
        // TODO: Add direction information
        if( runningVehicleIDs.find(vid) == runningVehicleIDs.end())
        {
          runningVehicleIDs.insert(vid);
        }
    }

    numOfVehicles = runningVehicleIDs.size();
  
}


/*
* --------------------------------
* updateArrTime()
* 
* According to each <prediction>, update the arrival time
* Update variables:
* - vehicles
*
* --------------------------------
*/
void updateArrTime(XMLElement * predictNode, String tag)
{
    String seconds = predictNode->Attribute("seconds");
    String minutes = predictNode->Attribute("minutes");
    String vid = predictNode->Attribute("vehicle");
    
    for (int i=0; i<numOfVehicles; i++)
    { 
        if(vehicles[i].vehicleID == vid && atoi((vehicles[i].arrTime).c_str()) >= atoi(minutes.c_str()))
        {
          vehicles[i].arrTime = minutes;
          vehicles[i].stopTag = tag;
          break;
        }
    }

}

/*
* --------------------------------
* readBusXML()
* 
* Get & analysis XMLContent
*
* Update variables:
* - vehicles
*
* --------------------------------
*/
void readBusXML()
{
    String url = "/predictions/trolley";

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Accept: application/xhtml+xml,application/xml\r\n"
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
    delay(2000);
    
    String line;
    String xmlContent = "";
   
    while (client.connected()) {
      
        line = client.readStringUntil('\n');

  //    Process predictions one by one
        if(line.startsWith("<predictions "))
        {   
            while(!line.startsWith("</predictions>"))
            {
                if(line.startsWith("  <message")) continue;
                xmlContent = xmlContent + line + "\n";
                line = client.readStringUntil('\n');
              }

            // Last line
            xmlContent = xmlContent + line;

            doc.Parse(xmlContent.c_str());
            XMLElement * predictionsNode = doc.FirstChildElement("predictions");
            String tag = predictionsNode->Attribute("stopTag");
            XMLElement * directionNode = predictionsNode->FirstChildElement("direction");
            String title = directionNode->Attribute("title");

            // The first predictions node
            if(numOfVehicles == 0)
            {
                getRunningVehicles(predictionsNode);
                
                set <String>::iterator it;
                String maxTime = "10000";
                int i=0;
                for(it=runningVehicleIDs.begin(); it!=runningVehicleIDs.end(); ++it, i++)
                {
                  vehicles[i].vehicleID = *it;
                  vehicles[i].stopTag = "NONE";
                  vehicles[i].arrTime = maxTime;
                  vehicles[i].direction = direction;
                  busLight[i] = false;
                }
            }

            Serial.println("At the beginning of readBusXML: ");
            Serial.println(numOfVehicles);

            XMLElement * predictNode = directionNode->FirstChildElement("prediction");
            // update time for each vehicle
            while (predictNode != NULL) // don't check direction (title == direction)
            {
                updateArrTime(predictNode, tag);
                predictNode = predictNode->NextSiblingElement("prediction");
            }
      
        }

        xmlContent = "";
        if(line.startsWith("</body>")) break;
    }
    
 }

// clear busLight flags, prevLightTimeStamps & light
void initState(){
    for (int i = 0; i < NUM_BUSSTOPS; i++)
      {
          busLight[i] = false;
      }
  
      for (int i = 0; i < numOfVehicles; i++)
      {
          prevLightTimeStamp[i] = millis();
      }
      
      for (int i = 0; i< NUMPIXELS; i++)
      {
          strip.setPixelColor(i, 0);
          strip.show();
      } 
}

void printOutBuses()
{
    Serial.println("=======================");
    Serial.println("Direction: " + direction);
    Serial.println("Num of Vehicles: ");
    Serial.println(numOfVehicles);

    // turn light on
    for(int i=0; i < numOfVehicles; i++)
    {
        Serial.println(vehicles[i].vehicleID);
        Serial.println(vehicles[i].stopTag);
        Serial.println(vehicles[i].arrTime);
        Serial.println(vehicles[i].direction);
    }
    Serial.println("");
  
}


void setup() {

    Serial.begin(115200);

    //Set up strip
    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP

    // Default route: trolley
    // Default direction: To MARTA Station
    // Route & direction could be updated by btn
    route  = "trolley";
    direction = "To MARTA Station";
    color = ORANGE_COLOR;

    // Initilize bus light
    initState();

    // initialize the pushbutton pin as an input:
    pinMode(BUTTONPIN, INPUT);

    rotateLightingStars_D1();

    /*
     * Setup app Request
     */
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


  
    const int port = 443;
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
//      return;
    }

    readBusXML();
    printOutBuses();

    // turn light on
    for(int i=0; i < numOfVehicles; i++)
    {
        int mint = atoi(vehicles[i].arrTime.c_str());
        
        if (mint == 0) 
        {
             int lightIndex = 0;
             for(int k = 0; k < NUMPIXELS; k++)
             {  
                if(STOP2INDEX[k] == vehicles[i].stopTag)
                {
                    lightIndex = k;
                    break;
                }
             }
             
             strip.setPixelColor(lightIndex, color);
             strip.show();
        }
        else
        {
            setLight(vehicles[i].stopTag);
        }
    }


    // Direction lights
    strip.setPixelColor(TROLLEY_D1_TO_MARTA, BLUE_COLOR);
    strip.show();
    
        
}

void setLight(String stopTag)
{
     int lightIndex = 0;
     for(int i = 0; i < NUMPIXELS; i++)
     {  
        if(STOP2INDEX[i] == stopTag)
        {
            lightIndex = i;
            break;
        }
     }

     if (busLight[lightIndex])
     {
        strip.setPixelColor(lightIndex, 0);
        strip.show();
        busLight[lightIndex] = false;
     }
     else
     {
        strip.setPixelColor(lightIndex, color);
        strip.show();
        busLight[lightIndex] = true;
     }
}


int curTime, preTime = millis();
int preBtnState, btnState = 0;
void updateLightStates()
{
    // Bus light blink
    for(int i=0; i < numOfVehicles; i++)
    {
        
        int mint = atoi(vehicles[i].arrTime.c_str());
        if (mint == 0) continue;
        else if(mint == 1)
        {
            if (curTime - prevLightTimeStamp[i] > 100)
            {
                setLight(vehicles[i].stopTag);
                prevLightTimeStamp[i] = curTime;
            }
        }
        else
        {
            if(curTime -  prevLightTimeStamp[i] > 1000/mint)
            {
                setLight(vehicles[i].stopTag);
                prevLightTimeStamp[i] = curTime;
            }
        }
    }
}

/*
 * light up from start to end 
 */
void rotateLightingStars_D2()
{
    int head = 0;

    while(++head <= NUMPIXELS)
    {
        strip.setPixelColor(head, color);
        strip.show();
        delay(50);
    }

    
    // Turn off all the lights
    head = 0;
    while(++head <= NUMPIXELS)
    {
        strip.setPixelColor(head, 0);
        strip.show();
    }
    
    strip.setPixelColor(TROLLEY_D2_TRANSIT_HUB, BLUE_COLOR);
    strip.show();
}

/*
 * light up from end to start 
 */
void rotateLightingStars_D1()
{
    int head = 24;

    while(--head >= 0)
    {
        strip.setPixelColor(head, color);
        strip.show();
        delay(50);
    }

    head = 24;
    while(--head >= 0)
    {
        strip.setPixelColor(head, 0);
        strip.show();
    }
    
    
    strip.setPixelColor(TROLLEY_D1_TO_MARTA, BLUE_COLOR);
    strip.show();
}

void updateDummyData()
{
    if(direction == TROLLEY_ROUTE_DIRECTION[1])
    {
        vehicles[0].vehicleID = "1";
        vehicles[0].stopTag = "wpe7mrt";
        vehicles[0].arrTime = "0";
        vehicles[0].direction = direction;
    
        vehicles[1].vehicleID = "2";
        vehicles[1].stopTag = "ferstdr";
        vehicles[1].arrTime = "1";
        vehicles[1].direction = direction;
    
        vehicles[2].vehicleID = "3";
        vehicles[2].stopTag = "fersforec";
        vehicles[2].arrTime = "4";
        vehicles[2].direction = direction;
    }
    else
    {
        vehicles[0].vehicleID = "1";
        vehicles[0].stopTag = "tech5mrt";
        vehicles[0].arrTime = "0";
        vehicles[0].direction = direction;
    
        vehicles[1].vehicleID = "1";
        vehicles[1].stopTag = "tranhub_a";
        vehicles[1].arrTime = "1";
        vehicles[1].direction = direction;
    
        vehicles[2].vehicleID = "3";
        vehicles[2].stopTag = "techsqua";
        vehicles[2].arrTime = "6";
        vehicles[2].direction = direction;
    }
}



void loop() {

    curTime = millis();
    // read the state of the pushbutton value:
    btnState = digitalRead(BUTTONPIN);
    updateLightStates(); // Could comment this when debugging mint=0

    
    // update button information  
    /*
     * Check button status
     */
    if(btnState == 1 && preBtnState == 0)
    {
        initState();
        updateDummyData();
        readBusXML();
        if(direction == TROLLEY_ROUTE_DIRECTION[1])
        {
            direction = TROLLEY_ROUTE_DIRECTION[0];
            rotateLightingStars_D1();
        }
        else
        {
            direction = TROLLEY_ROUTE_DIRECTION[1];
            rotateLightingStars_D2();
            
        }

        for(int i=0; i < numOfVehicles; i++)
        {
          int mint = atoi(vehicles[i].arrTime.c_str()); 
          if (mint == 0) 
          {
               int lightIndex = 0;
               for(int k = 0; k < NUMPIXELS; k++)
               {  
                  if(STOP2INDEX[k] == vehicles[i].stopTag)
                  {
                      lightIndex = k;
                      break;
                  }
               }
               // TODO:  need to fix bug for mint = 0 
               Serial.print("Light up light index is: ");
               Serial.println(lightIndex);
               strip.setPixelColor(lightIndex, color);
               strip.show();
          }
        }
        
        preTime = curTime;
        Serial.println("Direction has been changed to: " + direction);
    }
    preBtnState = btnState;


    /*
     * Send request every mins
     */
    if(curTime - preTime > 6000)
    {
        Serial.println("Send request again:");
        initState();
        readBusXML();
        printOutBuses();
        preTime = curTime;
    }
}

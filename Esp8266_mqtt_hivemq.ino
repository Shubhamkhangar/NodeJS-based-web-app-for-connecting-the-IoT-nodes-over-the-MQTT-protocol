/* 
  Name:       Shubham Anil Khangar
  Class:      M.Sc. (Electronics Science)
  University: Savitribai Phule Pune University
*/

// adding the required libraries here
#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #else
 #include <WiFi.h>
#endif

//#include<DHT.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/**** DHT11 sensor Settings *******/
//DHT dht(D1,DHT11);

/**** LED Settings *******/
int ledstate1;
int ledstate2;
int ledstate3;

String data = ""; //global variable to store the sensors data

/****** WiFi Connection Details *******/
const char* ssid = "Shubham";
const char* password = "876543210";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server   = "f27f91e6d8b24d02a7157ea968dd6c9d.s2.eu.hivemq.cloud";
const char* mqtt_username = "proj***";
const char* mqtt_password = "Proje***";
const int mqtt_port       = 8883;

/**** Secure WiFi Connectivity Initialisation *****/
WiFiClientSecure espClient;


/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

//function declaration 
void publishMessage(const char* topic, String payload , boolean retained);
void ledcheck(void);


/****** root certificate *********/

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

/************Connect to WiFi ***********/
void setup_wifi() 
{
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP()); //prints local IP address
}

/************* Connect to MQTT Broker ***********/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
     {
      Serial.println("connected with broker\n");
      //resubsription of the topics
      //client.subscribe("survey"); 
      client.subscribe("buttonReq");  
      delay(500);
      Serial.println("subscribed buttonReq from broker\n");
      ledcheck();
      delay(500);
      // preparing to send the survey responce to the mqtt broker 
      DynamicJsonDocument doc(1024);
         if(ledstate1)//led1
         {
          doc["led1"] = "ON";
         }
         else
         {
          doc["led1"] = "OFF";
         }

         if(ledstate2)//led2
         {
          doc["led2"] = "ON";
         }
         else
         {
          doc["led2"] = "OFF";
         }

        if(ledstate3)//led3
         {
          doc["led3"] = "ON";
         }
         else
         {
          doc["led3"] = "OFF";
         }
        
         doc["sensor"] = String(data);

        char mqtt_message[128];
        serializeJson(doc, mqtt_message); //converting to the json format
        Serial.print(mqtt_message);
        publishMessage("survey", mqtt_message, true); //publishing to the specified topic
        Serial.print("data published to survey topic\n");  
     }

    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/********** Call back Method for Receiving MQTT messages and Switching LED ********/

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) 
  incommingMessage+=(char)payload[i];
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

  //_____Processing incomming message__________________________________

    /*if( strcmp(topic,"project") == 0)
    {
     if (incommingMessage.equals("1"))
     { digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on
       float humidity = dht.readHumidity();
       float temperature = dht.readTemperature();
          DynamicJsonDocument doc(1024);
          doc["deviceId"] = "NodeMCU";
          doc["siteId"] = "My Demo Lab";
          doc["humidity"] = humidity;
          doc["temperature"] = temperature;
          char mqtt_message[128];
          serializeJson(doc, mqtt_message);
        publishMessage("project", mqtt_message, true);
     }*/
     
       if (incommingMessage.equals("button1 is pressed"))
       { Serial.println("incomming button 1");
        ledcheck();
         delay(500);
         ledstate1 = (ledstate1 == LOW) ? HIGH : LOW;
         digitalWrite(D2, ledstate1);
         DynamicJsonDocument doc(1024);
         if(ledstate1)
         {
          doc["led1"] = "ON";
         }
         else
         {
          doc["led1"] = "OFF";
         }

         if(ledstate2)
         {
          doc["led2"] = "ON";
         }
         else
         {
          doc["led2"] = "OFF";
         }

        if(ledstate3)
         {
          doc["led3"] = "ON";
         }
         else
         {
          doc["led3"] = "OFF";
         }
         doc["sensor"] = String(data);
        char mqtt_message[128];
        serializeJson(doc, mqtt_message);
        publishMessage("buttonRes", mqtt_message, true);
       }

       else if (incommingMessage.equals("button2 is pressed"))
       { Serial.println("incomming button 2");
         ledcheck();
         delay(500);
         ledstate2 = (ledstate2 == LOW) ? HIGH : LOW;
         digitalWrite(D3, ledstate2);
         DynamicJsonDocument doc(1024);
        if(ledstate1)
         {
          doc["led1"] = "ON";
         }
         else
         {
          doc["led1"] = "OFF";
         }

         if(ledstate2)
         {
          doc["led2"] = "ON";
         }
         else
         {
          doc["led2"] = "OFF";
         }

        if(ledstate3)
         {
          doc["led3"] = "ON";
         }
         else
         {
          doc["led3"] = "OFF";
         }
         doc["sensor"] = String(data);
        char mqtt_message[128];
        serializeJson(doc, mqtt_message);
        publishMessage("buttonRes", mqtt_message, true);
       }

       else if (incommingMessage.equals("button3 is pressed"))
       { Serial.println("incomming button 3");
         ledcheck();
         delay(500);
         ledstate3 = (ledstate3 == LOW) ? HIGH : LOW;
         digitalWrite(D4, ledstate3); //toggling led 
         DynamicJsonDocument doc(1024);
         if(ledstate1)
         {
          doc["led1"] = "ON";
         }
         else
         {
          doc["led1"] = "OFF";
         }

         if(ledstate2)
         {
          doc["led2"] = "ON";
         }
         else
         {
          doc["led2"] = "OFF";
         }

        if(ledstate3)
         {
          doc["led3"] = "ON";
         }
         else
         {
          doc["led3"] = "OFF";
         }
         doc["sensor"] = String(data);
        char mqtt_message[128];
        serializeJson(doc, mqtt_message);
        publishMessage("buttonRes", mqtt_message, true);
       }
  }



/******** Method for Publishing MQTT Messages **************/
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

/******* Application Initialisation Function ******/
void setup() {
  pinMode(D2, OUTPUT); //set up LED1
  pinMode(D3, OUTPUT); //set up LED2
  pinMode(D4, OUTPUT); //set up LED3
  //dht.begin();         //activating dht11 
  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();    //to connect with wifi network
  #ifdef ESP8266
    espClient.setInsecure();
  #else
    espClient.setCACert(root_ca);           // enable this line and the the "certificate" code for secure connection
  #endif
  client.setServer(mqtt_server, mqtt_port); // establishing the connection with mqtt server 
  client.setCallback(callback);             //setting the callback function to execute whenever the message from mqtt server recieves
}


/******** Main Function *************/
void loop() 
{
  if (!client.connected()) reconnect(); // check if client is connected
  client.loop();
  delay(500);
  int adcStep = analogRead(A0);
  float adcVolt = adcStep*0.00322265625;
       adcVolt = adcVolt*1000;
  float temperature = adcVolt/10;
  data = "Tempr"+(String)temperature;
  //float h=dht.readHumidity();
  //delay(100);
  //float t=dht.readTemperature();
  delay(100);
  //data = "HUM"+(String)h+"_"+"TEMP"+(String)t;   
}


void ledcheck(void)
{
 ledstate1 = digitalRead(D2);
 ledstate2 = digitalRead(D3);
 ledstate3 = digitalRead(D4);
}





















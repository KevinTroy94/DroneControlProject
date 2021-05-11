#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Time.h>
#include <TimeLib.h>


const char* ssid = "vodafone-BD85";
const char* password = "crushproof98";
const char* mqtt_server = "broker.emqx.io"; // the ip address of the pi/phone/device that is acting as the broker? tcp://broker.hivemq.com:1883 OR 192.168.1.5 /
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int numDronesInNetwork;
int droneNetworkID;
boolean initialised = false;
boolean toggledOn = true;
unsigned long currentMillis = millis();
double myTimeDouble;
String myTimeStr= "";
const char *charTime;

#define LED D1
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void determinePosition(char* topic, byte* payload){
  
  //determine poition
  //client.publish("formationTopic", ""+droneNetworkID);
  client.unsubscribe("droneOutTopic"); //unsubscribe to droneOutTopic
  Serial.println("Unsubscribed from droneOutTopic");
  //client.subscribe("formationTopic"); //subscribe to formation topic
  //client.publish("formationRequestTopic", "Find Me A Formation");
  
}

void callback(char* topic, byte* payload, unsigned int length) {

  if ((char)payload[0] == 'C'){
    
    //determinePosition(topic, payload);
  }
  String topicStr = topic;

  if(topicStr == "droneCommand/Toggle"){
    Serial.println("Toggle command received");
    Serial.print("payload ");
    Serial.println((char)payload[0]);
    Serial.print("myid ");
    Serial.println(droneNetworkID);
    int tempPayload = ((char)payload[0] - '0');
    int tempID = droneNetworkID;
    //tempA = tempB;
    if (tempPayload == tempID){
      Serial.println("and I know it's for me");
      if(toggledOn){
        Serial.println("toggling off");
        toggledOn = false;
      }
      else{
        Serial.println("toggling on");
        toggledOn = true;
      }
    }
    else{
      Serial.println("not for me though");
      Serial.println(tempPayload);
      Serial.println(tempID);
    }
    
  }

  if(topicStr == "formationAssignTopic"){
    //this is how the drones receive their number
    /*int numDronesInNetwork = (int)payload[0];
    int droneNetworkID = (int)payload[0];
    Serial.print("I am number ");
    
    Serial.print(" out of: ");
    Serial.print(numDronesInNetwork);
    if ((char)payload[0] == '1') { 
      
    }
    */
    Serial.print("Got format command");
    if ((char)payload[0] == '1') { 
      Serial.print("I am 1 of 1");
      numDronesInNetwork = 1;
      droneNetworkID = 1;
      client.unsubscribe("formationAssignTopic");
      
    }
    else if ((char)payload[0] == '2') { 
      Serial.print("I am 2 of 2");
      numDronesInNetwork = 2;
      droneNetworkID = 2;
      client.unsubscribe("formationAssignTopic");
    }
    else if ((char)payload[0] == '3') { 
      Serial.print("I am 3 of 3");
      numDronesInNetwork = 3;
      droneNetworkID = 3;
      client.unsubscribe("formationAssignTopic");
    }
  }

  if (topicStr == "DroneCommand/latency"){
    //Latency check received
    Serial.println("Latency Request received...");
    //currentMillis = millis();
    //Serial.println("Time in millis is ");
    currentMillis = millis();
    myTimeDouble = (double)currentMillis;
    char TempString[10];  //  Hold The Convert Data
    
    dtostrf(myTimeDouble,10,1,TempString);
    // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
    myTimeStr = String(TempString);  // cast it to string from char 
    charTime = myTimeStr.c_str();
    //const char *testChar = "258906.02";
    Serial.print("My Time is : ");
    Serial.println(charTime);

    client.publish("DroneCommand/latencyReply", charTime);
    
  }

  else if(topicStr == "DroneCommand/androidlatencyReply"){

    Serial.print("Latency reply received");

    currentMillis = millis();
    myTimeDouble = (double)currentMillis;
    char TempString[10];  //  Hold The Convert Data
    
    dtostrf(myTimeDouble,10,1,TempString);
    myTimeStr = String(TempString);  // cast it to string from char 
    charTime = myTimeStr.c_str(); // cast it to char
    Serial.print("My New Time is : ");
    Serial.println(charTime);

    //client.publish("DroneCommand/latencyReply", charTime);
    Serial.println("Your time is ");
    
  
  }

  //Serial.print(topicStr + " is the topic");
  
  /*Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } */
  //KEVIN's note: d0 is top left
  //KEVIN's note: d1 is top right
  //KEVIN's note: d2 is bottom right
  //KEVIN's note: d3 is bottom left
  if(toggledOn){
    if ((char)payload[7] == 'u') {   
      Serial.println("Going Forward");  
      
      /*digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D3, HIGH);*/ // test lights
  
      //d1 and d0 are top
      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      delay(500);
      
    } else if ((char)payload[7] == 'D') { 
       Serial.println("Going Back");   
       //d3 and d2 are bottom
      digitalWrite(D3, LOW);
      digitalWrite(D2, LOW);
      delay(500);
    }
    else if ((char)payload[7] == 'R') {
       Serial.println("Going Right");
       //d1 and d2 are top
      digitalWrite(D2, LOW);
      digitalWrite(D1, LOW);   
      delay(500);
    
    } else if ((char)payload[7] == 'L') {
       Serial.println("Going Left");  
       //d3 and d0 are LEFT
      digitalWrite(D0, LOW);
      digitalWrite(D3, LOW); 
      delay(500);
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("droneOutTopic", "Connecting");
      // ... and resubscribe
      client.subscribe("droneCommand/value");
      //client.subscribe("droneOutTopic");
      client.subscribe("DroneCommand/latency"); //latency topic
      //client.subscribe("DroneCommand/androidlatencyReply");
      client.subscribe("formationAssignTopic");
      client.subscribe("droneCommand/Toggle");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(D0, OUTPUT); // initialise top left
  digitalWrite(D0, LOW); 
  pinMode(D1, OUTPUT); // initialise top right
  digitalWrite(D1, LOW);
  pinMode(D2, OUTPUT); // initialise bottom left
  digitalWrite(D2, LOW);
  pinMode(D3, OUTPUT); // initialise bottom right
  digitalWrite(D3, LOW); 
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(!initialised){
    //determinePosition(topic, payload);
    client.publish("formationRequestTopic", "Find Me A Formation", 2); // upload with strict QOS 
    //client.publish("formationRequestTopic", "Find Me A Formation");
    Serial.println("published formatrequest");
    //come back to this
    initialised = true;
  }

  if(toggledOn){
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
  }
  else{
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
  }

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    Serial.println(msg);
    //client.publish("outTopic", msg);
    //client.publish("droneOutTopic", "Connecting");
  }
}

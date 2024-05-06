#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Deneyap_Servo.h>

Servo myservo;  

// Variable to control servo 
//https://nodered.org/docs/getting-started/windows#running-on-windows
//https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/



// Replace the next variables with your SSID/Password combination
const char* ssid = "Claro_077F98";
const char* password = "Q3K7M2R6K6S9";
#define servoPin 27

WiFiClient espClient;
PubSubClient client(espClient);



// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.20.46";




void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  int angle = messageTemp.toInt();
  myservo.write(angle);
  Serial.println();
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/servo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


unsigned long t_sta = 0;
unsigned long t_act = 0;
unsigned long t_ini = 0;


void setup() {

 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  myservo.attach(servoPin);
   
t_ini = millis();
Serial.println(t_ini);
}







void loop() {
  // put your main code here, to run repeatedly:




  if (!client.connected()) {
    reconnect();
   
  }
  client.loop();

Serial.println(millis()- t_ini);
  
  if (millis() - t_ini >= 5000)
  {
    /* code */

    int temp = random(24,30);
    char message[10]; // Assuming the message length is not greater than 10 characters
    sprintf(message, "%d", temp); // Convert integer to string
    
    client.publish("esp32/temperatura", message);
    Serial.println(message);

    t_ini = millis();
    
  }
  
  
  

}




#include <ESP8266WiFi.h>
#include <PubSubClient.h>


//#define trig D3
//#define echo D4
//#define nservo D5
// -------------------------------------------- 위의 헤더 파일 우노에 맞게 수정 
#define BTN1 D2
#define BTN2 D3
#define BTN3 D4
#define BTN4 D5
#define LED1 D6
#define LED2 D7
#define LED3 D11
#define LED4 D12

// 핀번호 우노에 맞게 수정 
// Update these with values suitable for your network.

//const char* ssid = "seozero";
//const char* password = "zero4443";

const char* ssid = "410-컴공 free 2G";
const char* password = "0548205480";


const char* mqtt_server = "broker.mqtt-dashboard.com";


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  //byte array를 string으로 바꿨다!
  
  String data;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
   data+=(char)payload[i];
  }
  //int degree = data.toInt();
  //int btn_mode = data.toInt();
  int tile1 = data.toInt();
  int tile2 = data.toInt();
  int tile3 = data.toInt();
  int tile4 = data.toInt();

  

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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("seoyeong/servo");
      client.subscribe("seoyeong/buttons");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// ------------------------------------------------------------------------필요 없음 위까지 


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(BTN1,INPUT);      //button1
  pinMode(BTN2,INPUT);      //button2
  pinMode(BTN3,INPUT);      //button3
  pinMode(BTN4,INPUT);      //button4
  
  pinMode(LED1,OUTPUT);     //led 1 
  pinMode(LED2,OUTPUT);     //led 2 
  pinMode(LED3,OUTPUT);     //led 3 
  pinMode(LED4,OUTPUT);     //led 4 

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  int arduino2 = 0;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(digitalRead(BTN1)==HIGH)
  {
    digitalWrite(LED1, HIGH);
     Serial.println(arduino2+5);
  }
  else
  {
    digitalWrite(LED1, LOW);
  }
  
  if(digitalRead(BTN2)==HIGH)
  {
    digitalWrite(LED2, HIGH);
    Serial.println(arduino2+15);
  }
  else
  {
    digitalWrite(LED2, LOW);
  }
  
  if(digitalRead(BTN3)==HIGH)
  {
    digitalWrite(LED3, HIGH);
    Serial.println(arduino2+25);
  }
  else
  {
    digitalWrite(LED3, LOW);
  }
  
  if(digitalRead(BTN4)==HIGH)
  {
    digitalWrite(LED4, HIGH);
    Serial.println(arduino2+35);
    //Serial.write(arduino2+35);
  }
  else
  {
    digitalWrite(LED4, LOW);
  }
  
}

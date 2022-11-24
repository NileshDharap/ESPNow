
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>

// Define DHT parameters
#define DHTPin 2
#define DHTType DHT11

// Create DHT Object
DHT dht(DHTPin, DHTType);

// Variables for temperature and humidity
float temp;
float humid;

// Integer for identification (make unique for each transmitter)
int ident = 1;

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD8, 0xF1, 0x5B, 0x13, 0x15, 0x48};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float a;
  float b;
  int c;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
  // Read DHT module values
  //temp = dht.readTemperature();
  temp = 27;
  delay(10);
  //humid = dht.readHumidity();
  humid = 77;
  delay(10);

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humid: ");
  Serial.println(humid);
  delay(1000);
  // Add to structured data object
  myData.a = temp;
  myData.b = humid;
  myData.c = ident;

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  lastTime = millis();
 }
}

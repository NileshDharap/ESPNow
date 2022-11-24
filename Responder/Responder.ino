
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    float a;
    float b;
    int c;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print to Serial Monitor
  Serial.print("Sensor: ");
  Serial.println(myData.c);
  Serial.print("Temperature: ");
  Serial.println(myData.a);
    
  Serial.print("Humidity: ");
  Serial.println(myData.b); 
  Serial.println("");
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Hello Nilesh!");
  display.display();
}

void loop() {
  
    display.setTextSize(1); 
    display.setCursor(0, 10);
    display.println("Sensor: ");
    display.print(myData.c);
    display.setCursor(0, 30);
    display.println("Temp: ");
    display.print(myData.a);
    display.print(" C");
    display.setCursor(0, 50);
    display.println("Hum: ");
    display.print(myData.b);
    display.print(" %");
    display.display();
}

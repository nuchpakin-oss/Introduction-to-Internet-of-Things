/*
  Project: Smart Classroom Focus Monitor
  Board: ESP32
  References:
  - DHT sensor library by Adafruit
  - PubSubClient by Nick O'Leary
  - ESP32Servo library
  - Arduino ESP32 WiFi examples

  What this code does:
  1) Reads DHT22, LDR, and PIR sensors
  2) Publishes sensor data as JSON to MQTT topic: classroom/sensors
  3) Subscribes to MQTT control topics for RGB LED, buzzer, servo, and mode
  4) Supports both AUTO logic from Node-RED and MANUAL control from dashboard/cloud
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ESP32Servo.h>

// ================= USER CONFIG =================
const char* WIFI_SSID     = "SirPakin";
const char* WIFI_PASSWORD = "12345689";

// If Node-RED/Mosquitto is on your laptop, use laptop IP, e.g. 192.168.1.10
const char* MQTT_SERVER = "broker.emqx.io";
const int   MQTT_PORT   = 1883;
const char* MQTT_CLIENT_ID = "esp32_smart_classroom";

// ================= PIN CONFIG =================
#define DHT_PIN      4
#define DHT_TYPE     DHT22
#define LDR_PIN      34     // ADC input only
#define PIR_PIN      27
#define LED_R_PIN    14
#define LED_G_PIN    12
#define LED_B_PIN    13
#define BUZZER_PIN   26
#define SERVO_PIN    25

// ================= MQTT TOPICS =================
const char* TOPIC_SENSOR       = "classroom/sensors";
const char* TOPIC_LED          = "classroom/control/led";      // RED/GREEN/BLUE/YELLOW/OFF
const char* TOPIC_BUZZER       = "classroom/control/buzzer";   // ON/OFF
const char* TOPIC_SERVO        = "classroom/control/servo";    // OPEN/CLOSE/0-180
const char* TOPIC_MODE         = "classroom/control/mode";     // AUTO/MANUAL
const char* TOPIC_HEARTBEAT    = "classroom/status/esp32";

WiFiClient espClient;
PubSubClient mqtt(espClient);
DHT dht(DHT_PIN, DHT_TYPE);
Servo curtainServo;

unsigned long lastPublish = 0;
const unsigned long PUBLISH_INTERVAL_MS = 3000;
String currentMode = "AUTO";

void setRGB(bool r, bool g, bool b) {
  digitalWrite(LED_R_PIN, r ? HIGH : LOW);
  digitalWrite(LED_G_PIN, g ? HIGH : LOW);
  digitalWrite(LED_B_PIN, b ? HIGH : LOW);
}

void setLEDColor(String color) {
  color.toUpperCase();
  if (color == "RED") setRGB(true, false, false);
  else if (color == "GREEN") setRGB(false, true, false);
  else if (color == "BLUE") setRGB(false, false, true);
  else if (color == "GREEN") setRGB(true, true, false);
  else if (color == "CYAN") setRGB(false, true, true);
  else if (color == "MAGENTA") setRGB(true, false, true);
  else setRGB(false, false, false);
}

void setBuzzer(String state) {
  state.toUpperCase();
  digitalWrite(BUZZER_PIN, state == "ON" ? HIGH : LOW);
}

void setServo(String command) {
  command.toUpperCase();
  if (command == "OPEN") curtainServo.write(90);
  else if (command == "CLOSE") curtainServo.write(0);
  else {
    int angle = command.toInt();
    if (angle >= 0 && angle <= 180) curtainServo.write(angle);
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  String t = String(topic);

  if (t == TOPIC_LED) setLEDColor(msg);
  else if (t == TOPIC_BUZZER) setBuzzer(msg);
  else if (t == TOPIC_SERVO) setServo(msg);
  else if (t == TOPIC_MODE) {
    msg.toUpperCase();
    if (msg == "AUTO" || msg == "MANUAL") currentMode = msg;
  }
}

void connectWiFi() {
  Serial.print("Connecting WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting MQTT...");
    if (mqtt.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
      mqtt.subscribe(TOPIC_LED);
      mqtt.subscribe(TOPIC_BUZZER);
      mqtt.subscribe(TOPIC_SERVO);
      mqtt.subscribe(TOPIC_MODE);
      mqtt.publish(TOPIC_HEARTBEAT, "ESP32 online");
    } else {
      Serial.print("failed rc=");
      Serial.print(mqtt.state());
      Serial.println(" retry in 3s");
      delay(3000);
    }
  }
}

int readLightPercent() {
  int raw = analogRead(LDR_PIN);        // 0-4095
  int percent = map(raw, 0, 4095, 0, 100);
  // Depending on your voltage divider wiring, value may be inverted.
  // If light/dark is reversed, replace the line above with: map(raw, 0, 4095, 100, 0)
  return constrain(percent, 0, 100);
}

void publishSensors() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int lightPercent = readLightPercent();
  int motion = digitalRead(PIR_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT read failed");
    return;
  }

  char payload[256];
  snprintf(payload, sizeof(payload),
           "{\"temperature\":%.2f,\"humidity\":%.2f,\"light\":%d,\"motion\":%d,\"mode\":\"%s\"}",
           temp, hum, lightPercent, motion, currentMode.c_str());

  mqtt.publish(TOPIC_SENSOR, payload);
  Serial.println(payload);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setLEDColor("OFF");
  setBuzzer("OFF");

  curtainServo.attach(SERVO_PIN);
  curtainServo.write(0);

  connectWiFi();
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();

  if (millis() - lastPublish >= PUBLISH_INTERVAL_MS) {
    lastPublish = millis();
    publishSensors();
  }
}

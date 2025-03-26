#include "M5Atom.h"
#include "AtomSocket.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define RXD   22
#define RELAY 23

ATOMSOCKET ATOM;
HardwareSerial AtomSerial(2);

// WiFi and MQTT configuration
#define WIFI_SSID "CHANGEME"
#define WIFI_PASSWORD "*CHANGEME"
#define MQTT_DEVICE "atom1"
#define MQTT_BROKER "192.0.0.0"
#define MQTT_PORT 1883
#define MQTT_USER "CHANGEME"
#define MQTT_PASSWORD "CHANGEME"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool RelayFlag = true;

void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}

void registerWithHomeAssistant() {
    // JSON payload for Home Assistant Discovery
    String discoveryTopic = String("homeassistant/switch/") + MQTT_DEVICE + "/config";
    String stateTopic = String("homeassistant/switch/") + MQTT_DEVICE + "/state";
    String commandTopic = String("homeassistant/switch/") + MQTT_DEVICE + "/set";

    String discoveryPayload = R"({
        "name": "Atom Relay",
        "state_topic": ")" + stateTopic + R"(",
        "command_topic": ")" + commandTopic + R"(",
        "payload_on": "ON",
        "payload_off": "OFF",
        "state_on": "ON",
        "state_off": "OFF",
        "unique_id": ")" + MQTT_DEVICE + R"(",
        "device": {
            "identifiers": [")" + MQTT_DEVICE + R"("],
            "name": "Atom Relay",
            "model": "M5Atom",
            "manufacturer": "M5Stack"
        }
    })";

    mqttClient.publish(discoveryTopic.c_str(), discoveryPayload.c_str(), true);
    Serial.println("Device registered with Home Assistant");
}

void connectMQTT() {
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

    // Increase buffer size to handle larger payloads
    mqttClient.setBufferSize(512);

    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect(MQTT_DEVICE, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Connected to MQTT");
            // Subscribe to control topic
            String commandTopic = String("homeassistant/switch/") + MQTT_DEVICE + "/set";
            mqttClient.subscribe(commandTopic.c_str());

            // Register device with Home Assistant
            registerWithHomeAssistant();
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

// Centralized method to manage state and publish updates
void setDeviceState(bool state) {
    RelayFlag = state;

    // Update relay state
    if (RelayFlag) {
        M5.dis.drawpix(0, 0x00ff00);
        ATOM.SetPowerOn();
    } else {
        M5.dis.drawpix(0, 0xff0000);
        ATOM.SetPowerOff();
    }

    // Publish the updated state to Home Assistant
    String stateTopic = String("homeassistant/switch/") + MQTT_DEVICE + "/state";
    mqttClient.publish(stateTopic.c_str(), RelayFlag ? "ON" : "OFF", true);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Received message: ");
    Serial.println(message);

    if (message == "ON") {
        setDeviceState(true);
    } else if (message == "OFF") {
        setDeviceState(false);
    }
}

void setup() {
    M5.begin(true, false, true);
    delay(1000);
    ATOM.Init(AtomSerial, RELAY, RXD);
    Serial.begin(115200);

    // Initialize WiFi and MQTT
    connectWiFi();
    mqttClient.setCallback(mqttCallback);
    connectMQTT();

    // Register initial state
    setDeviceState(RelayFlag);

    Serial.println("ATOM started");
}

void loop() {
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    mqttClient.loop();

    // Handle button press
    if (M5.Btn.wasPressed()) {
        Serial.println("Button Pressed");
        setDeviceState(!RelayFlag);
    }

    M5.update();
}

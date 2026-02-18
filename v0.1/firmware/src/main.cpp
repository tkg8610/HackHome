#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include "config.h"

// ----- Globals -----
AsyncWebServer server(80);
Servo servo;
CRGB leds[1];

int currentAngle = SERVO_REST_ANGLE;
bool isPressing = false;
unsigned long lastButtonPress = 0;

// ----- LED helpers -----
void setLED(CRGB color) {
    leds[0] = color;
    FastLED.show();
}

// ----- Servo control -----
void setServoAngle(int angle) {
    angle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    servo.write(angle);
    currentAngle = angle;
}

void servoPress() {
    if (isPressing) return;
    isPressing = true;
    setLED(CRGB::Yellow);
    setServoAngle(SERVO_PRESS_ANGLE);
    delay(SERVO_PRESS_DELAY);
    setServoAngle(SERVO_REST_ANGLE);
    setLED(CRGB::Green);
    isPressing = false;
}

// ----- WiFi -----
void connectWiFi() {
    Serial.printf("Connecting to %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        setLED(CRGB::Blue);
        delay(300);
        setLED(CRGB::Black);
        delay(300);
        Serial.print(".");
    }

    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
    setLED(CRGB::Green);
}

// ----- API routes -----
void setupRoutes() {
    // CORS preflight
    server.on("/*", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(204);
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");
        request->send(response);
    });

    // GET /api/status
    server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument doc;
        doc["angle"] = currentAngle;
        doc["pressing"] = isPressing;
        doc["rssi"] = WiFi.RSSI();
        doc["ip"] = WiFi.localIP().toString();

        String json;
        serializeJson(doc, json);

        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json);
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    // POST /api/servo/press
    server.on("/api/servo/press", HTTP_POST, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{\"ok\":true}");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
        servoPress();
    });

    // POST /api/servo/angle
    server.on("/api/servo/angle", HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            JsonDocument doc;
            deserializeJson(doc, data, len);
            int angle = doc["angle"] | -1;

            if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
                AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"error\":\"invalid angle\"}");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                return;
            }

            setServoAngle(angle);

            JsonDocument res;
            res["ok"] = true;
            res["angle"] = currentAngle;
            String json;
            serializeJson(res, json);

            AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
    );
}

// ----- Setup -----
void setup() {
    Serial.begin(115200);

    // LED
    FastLED.addLeds<WS2812, PIN_LED, GRB>(leds, 1);
    FastLED.setBrightness(30);
    setLED(CRGB::Red);

    // Button
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    // Servo
    servo.attach(PIN_SERVO);
    setServoAngle(SERVO_REST_ANGLE);

    // WiFi
    connectWiFi();

    // mDNS
    if (MDNS.begin(MDNS_HOSTNAME)) {
        Serial.printf("mDNS: http://%s.local\n", MDNS_HOSTNAME);
    }

    // API
    setupRoutes();
    server.begin();
    Serial.println("Server started");
}

// ----- Loop -----
void loop() {
    // Button handling
    if (digitalRead(PIN_BUTTON) == LOW) {
        unsigned long now = millis();
        if (now - lastButtonPress > DEBOUNCE_MS) {
            lastButtonPress = now;
            servoPress();
        }
    }

    // Reconnect WiFi if disconnected
    if (WiFi.status() != WL_CONNECTED) {
        setLED(CRGB::Red);
        connectWiFi();
    }
}

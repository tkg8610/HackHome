#pragma once

// ===== WiFi Settings =====
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASSWORD"

// ===== Pin Definitions (M5Stamp S3) =====
#define PIN_SERVO    1   // GPIO1 - Servo PWM
#define PIN_BUTTON   0   // GPIO0 - Built-in button
#define PIN_LED      21  // GPIO21 - Built-in WS2812 RGB LED

// ===== Servo Settings =====
#define SERVO_MIN_ANGLE  0
#define SERVO_MAX_ANGLE  180
#define SERVO_PRESS_ANGLE 90   // Angle when "pressing" switch
#define SERVO_REST_ANGLE  0    // Resting angle
#define SERVO_PRESS_DELAY 500  // ms to hold press position

// ===== mDNS =====
#define MDNS_HOSTNAME "hackhome"

// ===== Button Debounce =====
#define DEBOUNCE_MS 200

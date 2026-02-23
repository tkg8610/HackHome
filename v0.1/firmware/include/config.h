#pragma once

// ===== WiFi Settings =====
// WIFI_SSID and WIFI_PASS are loaded from secrets.txt via build flags.
// Create v0.1/firmware/secrets.txt with:
//   Line 1: SSID
//   Line 2: Password
#ifndef WIFI_SSID
#error "secrets.txt not found! Create it in v0.1/firmware/ with SSID on line 1 and password on line 2."
#endif

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

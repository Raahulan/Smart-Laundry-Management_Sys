#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ========================================
// SMART LAUNDRY
// MANUAL + WIFI + THINGSBOARD
// ========================================


// ========================================
// WIFI DETAILS
// ========================================

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";


// ========================================
// THINGSBOARD DETAILS
// ========================================

const char* THINGSBOARD_SERVER = "mqtt.thingsboard.cloud";
const int THINGSBOARD_PORT = 1883;

const char* ACCESS_TOKEN =
  "YOUR_THINGSBOARD_ACCESS_TOKEN";


// ========================================
// RELAYS
// ========================================

#define RELAY1 13
#define RELAY2 14
#define RELAY3 27
#define RELAY4 26


// ========================================
// BUTTONS
// ========================================

#define BUTTON1 21
#define BUTTON2 22
#define BUTTON3 15
#define BUTTON4 12


// ========================================
// BUZZER
// ========================================

#define BUZZER 25


// ========================================
// TFT
// ========================================

#define TFT_CS   5
#define TFT_RST  4
#define TFT_DC   2

// SPI:
// MOSI = GPIO23
// SCK  = GPIO18

Adafruit_ST7735 tft =
  Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


// ========================================
// RELAY LOGIC
// Most relay modules are ACTIVE LOW
// ========================================

#define RELAY_ON  LOW
#define RELAY_OFF HIGH


int relayPins[4] = {
  RELAY1,
  RELAY2,
  RELAY3,
  RELAY4
};

int buttonPins[4] = {
  BUTTON1,
  BUTTON2,
  BUTTON3,
  BUTTON4
};


// true = AVAILABLE
// false = WORKING

bool machineState[4] = {
  true,
  true,
  true,
  true
};


// ========================================
// WIFI / MQTT
// ========================================

WiFiClient wifiClient;

PubSubClient mqttClient(
  wifiClient
);


// ========================================
// TELEMETRY TIMER
// ========================================

unsigned long lastTelemetryTime = 0;

const unsigned long telemetryInterval = 3000;


// ========================================
// COUNT FREE MACHINES
// ========================================

int countFreeMachines() {

  int freeCount = 0;

  for (int i = 0; i < 4; i++) {

    if (machineState[i] == true) {
      freeCount++;
    }
  }

  return freeCount;
}


// ========================================
// SEND TELEMETRY
// ========================================

void sendTelemetry() {

  if (!mqttClient.connected()) {
    return;
  }

  StaticJsonDocument<512> doc;


  doc["machine1"] =
    machineState[0] ?
    "AVAILABLE" :
    "WORKING";

  doc["machine2"] =
    machineState[1] ?
    "AVAILABLE" :
    "WORKING";

  doc["machine3"] =
    machineState[2] ?
    "AVAILABLE" :
    "WORKING";

  doc["machine4"] =
    machineState[3] ?
    "AVAILABLE" :
    "WORKING";


  doc["machine1_available"] =
    machineState[0];

  doc["machine2_available"] =
    machineState[1];

  doc["machine3_available"] =
    machineState[2];

  doc["machine4_available"] =
    machineState[3];


  doc["available_count"] =
    countFreeMachines();


  char buffer[512];

  serializeJson(
    doc,
    buffer
  );


  mqttClient.publish(
    "v1/devices/me/telemetry",
    buffer
  );


  Serial.println(
    "Telemetry sent to ThingsBoard"
  );
}


// ========================================
// CONTROL MACHINE
// ========================================

void controlMachine(int machine) {

  if (machineState[machine] == true) {

    machineState[machine] = false;

    digitalWrite(
      relayPins[machine],
      RELAY_OFF
    );

  } else {

    machineState[machine] = true;

    digitalWrite(
      relayPins[machine],
      RELAY_ON
    );
  }


  Serial.print("Machine ");
  Serial.print(machine + 1);
  Serial.print(" = ");

  if (machineState[machine]) {

    Serial.println("WORKING");

  } else {

    Serial.println("AVAILABLE");
  }


  updateDisplay();

  sendTelemetry();
}


// ========================================
// REMOTE CONTROL FUNCTION
// ========================================

void setMachineRemote(
  int machine,
  bool available
) {

  machineState[machine] =
    available;


  if (machineState[machine] == true) {

    digitalWrite(
      relayPins[machine],
      RELAY_ON
    );

  } else {

    digitalWrite(
      relayPins[machine],
      RELAY_OFF
    );
  }


  Serial.print("Remote Machine ");
  Serial.print(machine + 1);
  Serial.print(" = ");

  if (machineState[machine]) {

    Serial.println("AVAILABLE");

  } else {

    Serial.println("WORKING");
  }


  updateDisplay();

  sendTelemetry();
}


// ========================================
// TFT DISPLAY
// ========================================

void updateDisplay() {

  tft.fillScreen(ST77XX_BLACK);


  // TITLE
  tft.setTextSize(2);

  tft.setTextColor(ST77XX_CYAN);

  tft.setCursor(8, 3);

  tft.print("LAUNDRY");


  tft.drawLine(
    0,
    23,
    160,
    23,
    ST77XX_WHITE
  );


  // ======================================
  // MACHINE 1
  // ======================================

  tft.setCursor(3, 30);

  tft.setTextColor(ST77XX_WHITE);

  tft.print("M1:");

  if (machineState[0]) {

    tft.setTextColor(ST77XX_GREEN);
    tft.print("AVAILABLE");

  } else {

    tft.setTextColor(ST77XX_RED);
    tft.print("WORKING");
  }


  // ======================================
  // MACHINE 2
  // ======================================

  tft.setCursor(3, 52);

  tft.setTextColor(ST77XX_WHITE);

  tft.print("M2:");

  if (machineState[1]) {

    tft.setTextColor(ST77XX_GREEN);
    tft.print("AVAILABLE");

  } else {

    tft.setTextColor(ST77XX_RED);
    tft.print("WORKING");
  }


  // ======================================
  // MACHINE 3
  // ======================================

  tft.setCursor(3, 74);

  tft.setTextColor(ST77XX_WHITE);

  tft.print("M3:");

  if (machineState[2]) {

    tft.setTextColor(ST77XX_GREEN);
    tft.print("AVAILABLE");

  } else {

    tft.setTextColor(ST77XX_RED);
    tft.print("WORKING");
  }


  // ======================================
  // MACHINE 4
  // ======================================

  tft.setCursor(3, 96);

  tft.setTextColor(ST77XX_WHITE);

  tft.print("M4:");

  if (machineState[3]) {

    tft.setTextColor(ST77XX_GREEN);
    tft.print("AVAILABLE");

  } else {

    tft.setTextColor(ST77XX_RED);
    tft.print("WORKING");
  }


  // ======================================
  // AVAILABLE COUNT
  // ======================================

  tft.drawLine(
    0,
    118,
    160,
    118,
    ST77XX_WHITE
  );


  tft.setCursor(3, 125);

  tft.setTextColor(ST77XX_YELLOW);

  tft.print("AVAILABLE:");

  tft.print(
    countFreeMachines()
  );
}


// ========================================
// WIFI CONNECTION
// ========================================

void connectWiFi() {

  Serial.println();

  Serial.print(
    "Connecting to WiFi: "
  );

  Serial.println(
    WIFI_SSID
  );


  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );


  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }


  Serial.println();

  Serial.println(
    "WiFi Connected"
  );


  Serial.print(
    "ESP32 IP: "
  );

  Serial.println(
    WiFi.localIP()
  );
}


// ========================================
// THINGSBOARD RPC CALLBACK
// ========================================

void mqttCallback(
  char* topic,
  byte* payload,
  unsigned int length
) {

  String message = "";


  for (
    unsigned int i = 0;
    i < length;
    i++
  ) {

    message +=
      (char)payload[i];
  }


  Serial.println();

  Serial.print(
    "ThingsBoard RPC: "
  );

  Serial.println(
    message
  );


  StaticJsonDocument<256> doc;


  DeserializationError error =
    deserializeJson(
      doc,
      message
    );


  if (error) {

    Serial.println(
      "JSON Parse Error"
    );

    return;
  }


  const char* method =
    doc["method"];


  bool value =
    doc["params"];


  if (method == NULL) {

    return;
  }


  // ======================================
  // MACHINE 1
  // ======================================

  if (
    strcmp(
      method,
      "setMachine1"
    ) == 0
  ) {

    setMachineRemote(
      0,
      value
    );
  }


  // ======================================
  // MACHINE 2
  // ======================================

  else if (
    strcmp(
      method,
      "setMachine2"
    ) == 0
  ) {

    setMachineRemote(
      1,
      value
    );
  }


  // ======================================
  // MACHINE 3
  // ======================================

  else if (
    strcmp(
      method,
      "setMachine3"
    ) == 0
  ) {

    setMachineRemote(
      2,
      value
    );
  }


  // ======================================
  // MACHINE 4
  // ======================================

  else if (
    strcmp(
      method,
      "setMachine4"
    ) == 0
  ) {

    setMachineRemote(
      3,
      value
    );
  }
}


// ========================================
// THINGSBOARD CONNECTION
// ========================================

void connectThingsBoard() {

  while (
    !mqttClient.connected()
  ) {

    Serial.println(
      "Connecting to ThingsBoard..."
    );


    String clientID =
      "SmartLaundry_";

    clientID +=
      String(
        random(0xffff),
        HEX
      );


    if (
      mqttClient.connect(
        clientID.c_str(),
        ACCESS_TOKEN,
        NULL
      )
    ) {

      Serial.println(
        "ThingsBoard Connected"
      );


      mqttClient.subscribe(
        "v1/devices/me/rpc/request/+"
      );


      sendTelemetry();

    } else {

      Serial.print(
        "MQTT Failed. State: "
      );

      Serial.println(
        mqttClient.state()
      );


      delay(3000);
    }
  }
}


// ========================================
// SETUP
// ========================================

void setup() {

  Serial.begin(115200);


  // -------------------------
  // RELAYS
  // -------------------------

  for (int i = 0; i < 4; i++) {

    pinMode(
      relayPins[i],
      OUTPUT
    );

    digitalWrite(
      relayPins[i],
      RELAY_ON
    );

    machineState[i] = true;
  }


  // -------------------------
  // BUTTONS
  // -------------------------

  for (int i = 0; i < 4; i++) {

    pinMode(
      buttonPins[i],
      INPUT_PULLUP
    );
  }


  // -------------------------
  // BUZZER
  // -------------------------

  pinMode(
    BUZZER,
    OUTPUT
  );

  digitalWrite(
    BUZZER,
    LOW
  );


  // -------------------------
  // TFT
  // -------------------------

  tft.initR(
    INITR_BLACKTAB
  );

  tft.setRotation(1);

  tft.fillScreen(
    ST77XX_BLACK
  );


  tft.setTextColor(
    ST77XX_WHITE
  );

  tft.setTextSize(2);

  tft.setCursor(
    10,
    35
  );

  tft.print(
    "SMART"
  );

  tft.setCursor(
    10,
    60
  );

  tft.print(
    "LAUNDRY"
  );

  delay(1000);


  updateDisplay();


  Serial.println();

  Serial.println(
    "SYSTEM READY"
  );

  Serial.println(
    "ALL MACHINES WORKING"
  );


  // ========================================
  // WIFI
  // ========================================

  connectWiFi();


  // ========================================
  // MQTT
  // ========================================

  mqttClient.setServer(
    THINGSBOARD_SERVER,
    THINGSBOARD_PORT
  );


  mqttClient.setCallback(
    mqttCallback
  );


  connectThingsBoard();
}


// ========================================
// LOOP
// ========================================

void loop() {

  // ========================================
  // WIFI RECONNECT
  // ========================================

  if (
    WiFi.status() != WL_CONNECTED
  ) {

    connectWiFi();
  }


  // ========================================
  // THINGSBOARD RECONNECT
  // ========================================

  if (
    !mqttClient.connected()
  ) {

    connectThingsBoard();
  }


  mqttClient.loop();


  // ========================================
  // BUTTON 1
  // ========================================

  if (digitalRead(BUTTON1) == LOW) {

    delay(50);

    if (digitalRead(BUTTON1) == LOW) {

      controlMachine(0);

      while (
        digitalRead(BUTTON1) == LOW
      ) {
        delay(10);
      }

      delay(100);
    }
  }


  // ========================================
  // BUTTON 2
  // ========================================

  if (digitalRead(BUTTON2) == LOW) {

    delay(50);

    if (digitalRead(BUTTON2) == LOW) {

      controlMachine(1);

      while (
        digitalRead(BUTTON2) == LOW
      ) {
        delay(10);
      }

      delay(100);
    }
  }


  // ========================================
  // BUTTON 3
  // ========================================

  if (digitalRead(BUTTON3) == LOW) {

    delay(50);

    if (digitalRead(BUTTON3) == LOW) {

      controlMachine(2);

      while (
        digitalRead(BUTTON3) == LOW
      ) {
        delay(10);
      }

      delay(100);
    }
  }


  // ========================================
  // BUTTON 4
  // ========================================

  if (digitalRead(BUTTON4) == LOW) {

    delay(50);

    if (digitalRead(BUTTON4) == LOW) {

      controlMachine(3);

      while (
        digitalRead(BUTTON4) == LOW
      ) {
        delay(10);
      }

      delay(100);
    }
  }


  // ========================================
  // PERIODIC TELEMETRY
  // ========================================

  if (
    millis() - lastTelemetryTime
    >= telemetryInterval
  ) {

    lastTelemetryTime =
      millis();

    sendTelemetry();
  }
}
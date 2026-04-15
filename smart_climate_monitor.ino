
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -----------------------------------------------
//   GPIO PINS
// -----------------------------------------------
#define BTN_NEXT   21
#define BTN_UNIT   5

// -----------------------------------------------
//   OLED SETTINGS
// -----------------------------------------------
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDRESS  0x3C

// -----------------------------------------------
//   OBJECTS
// -----------------------------------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0 aht;

// -----------------------------------------------
//   SCREEN STATE
// -----------------------------------------------
int currentScreen = 0;
const int TOTAL_SCREENS = 5;
bool useFahrenheit = false;

// -----------------------------------------------
//   BUTTON DEBOUNCE
// -----------------------------------------------
unsigned long lastDebounce1 = 0;
unsigned long lastDebounce2 = 0;
const unsigned long DEBOUNCE_DELAY = 200;
bool lastState1 = HIGH;
bool lastState2 = HIGH;

// -----------------------------------------------
//   SENSOR DATA
// -----------------------------------------------
float temperature = 0.0;
float humidity    = 0.0;
float heatIndex   = 0.0;

float minTemp = 999.0;
float maxTemp = -999.0;
float minHum  = 999.0;
float maxHum  = -999.0;

// -----------------------------------------------
//   HELPER: CENTER TEXT
// -----------------------------------------------
void printCentered(String text, int y, int textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

// -----------------------------------------------
//   CELSIUS TO FAHRENHEIT
// -----------------------------------------------
float toFahrenheit(float c) {
  return (c * 9.0 / 5.0) + 32.0;
}

// -----------------------------------------------
//   HEAT INDEX CALCULATION
// -----------------------------------------------
float computeHeatIndex(float t, float h) {
  float hi = -8.78469475556
           + 1.61139411    * t
           + 2.33854883889 * h
           - 0.14611605    * t * h
           - 0.012308094   * t * t
           - 0.0164248278  * h * h
           + 0.002211732   * t * t * h
           + 0.00072546    * t * h * h
           - 0.000003582   * t * t * h * h;
  return hi;
}

// -----------------------------------------------
//   COMFORT STATUS
// -----------------------------------------------
String getComfortStatus(float t, float h) {
  if (t > 35)  return "Too Hot!";
  if (t < 18)  return "Too Cold!";
  if (h > 70)  return "Too Humid!";
  if (h < 30)  return "Too Dry!";
  return "Comfortable :)";
}

// -----------------------------------------------
//   READ SENSOR & UPDATE MIN/MAX
// -----------------------------------------------
void readSensor() {
  sensors_event_t hum_event, temp_event;
  aht.getEvent(&hum_event, &temp_event);
  temperature = temp_event.temperature;
  humidity    = hum_event.relative_humidity;
  heatIndex   = computeHeatIndex(temperature, humidity);

  if (temperature < minTemp) minTemp = temperature;
  if (temperature > maxTemp) maxTemp = temperature;
  if (humidity < minHum)     minHum  = humidity;
  if (humidity > maxHum)     maxHum  = humidity;
}

// -----------------------------------------------
//   BUTTON CHECK HELPER
// -----------------------------------------------
bool isPressed(int pin, bool &lastState, unsigned long &lastDebounce) {
  bool state = digitalRead(pin);
  if (state == LOW && lastState == HIGH) {
    if (millis() - lastDebounce > DEBOUNCE_DELAY) {
      lastDebounce = millis();
      lastState = state;
      return true;
    }
  }
  lastState = state;
  return false;
}

// -----------------------------------------------
//   SCREEN 1 — TEMPERATURE
// -----------------------------------------------
void showTemperature() {
  display.clearDisplay();
  printCentered("[ TEMPERATURE ]", 4, 1);
  float dispTemp = useFahrenheit ? toFahrenheit(temperature) : temperature;
  char unit = useFahrenheit ? 'F' : 'C';
  String val = String(dispTemp, 1) + (char)247 + unit;
  printCentered(val, 22, 2);
  printCentered("NEXT     C/F", 56, 1);
  display.display();
}

// -----------------------------------------------
//   SCREEN 2 — HUMIDITY
// -----------------------------------------------
void showHumidity() {
  display.clearDisplay();
  printCentered("[ HUMIDITY ]", 4, 1);
  String val = String(humidity, 1) + " %";
  printCentered(val, 22, 2);
  printCentered("NEXT     C/F", 56, 1);
  display.display();
}

// -----------------------------------------------
//   SCREEN 3 — HEAT INDEX
// -----------------------------------------------
void showHeatIndex() {
  display.clearDisplay();
  printCentered("[ HEAT INDEX ]", 4, 1);
  float dispHI = useFahrenheit ? toFahrenheit(heatIndex) : heatIndex;
  char unit = useFahrenheit ? 'F' : 'C';
  String val = String(dispHI, 1) + (char)247 + unit;
  printCentered(val, 22, 2);
  printCentered("NEXT     C/F", 56, 1);
  display.display();
}

// -----------------------------------------------
//   SCREEN 4 — STATUS
// -----------------------------------------------
void showStatus() {
  display.clearDisplay();
  printCentered("[ STATUS ]", 4, 1);
  String status = getComfortStatus(temperature, humidity);
  printCentered(status, 22, status.length() > 10 ? 1 : 2);
  float dispTemp = useFahrenheit ? toFahrenheit(temperature) : temperature;
  char unit = useFahrenheit ? 'F' : 'C';
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print("T:");
  display.print(dispTemp, 1);
  display.print((char)247);
  display.print(unit);
  display.print("  H:");
  display.print(humidity, 1);
  display.print("%");
  printCentered("NEXT     C/F", 56, 1);
  display.display();
}

// -----------------------------------------------
//   SCREEN 5 — MIN/MAX
// -----------------------------------------------
void showMinMax() {
  display.clearDisplay();
  printCentered("[ MIN / MAX ]", 2, 1);
  float dMinT = useFahrenheit ? toFahrenheit(minTemp) : minTemp;
  float dMaxT = useFahrenheit ? toFahrenheit(maxTemp) : maxTemp;
  char unit   = useFahrenheit ? 'F' : 'C';
  display.setTextSize(1);
  display.setCursor(0, 17);
  display.print("Temp MIN: ");
  display.print(dMinT, 1);
  display.print((char)247);
  display.println(unit);
  display.setCursor(0, 28);
  display.print("Temp MAX: ");
  display.print(dMaxT, 1);
  display.print((char)247);
  display.println(unit);
  display.setCursor(0, 39);
  display.print("Hum  MIN: ");
  display.print(minHum, 1);
  display.println("%");
  display.setCursor(0, 50);
  display.print("Hum  MAX: ");
  display.print(maxHum, 1);
  display.println("%");
  display.display();
}

// -----------------------------------------------
//   SETUP
// -----------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_UNIT, INPUT_PULLUP);

  Wire.begin(8, 9);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("ERROR: OLED not found!");
    while (true);
  }

  // Splash Screen
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  printCentered("Smart Climate", 8, 1);
  printCentered("Monitor v3.0", 22, 1);
  printCentered("Sofia L. Pardillo", 40, 1);
  printCentered("BSICE - 3", 52, 1);
  display.display();
  delay(2500);

  if (!aht.begin()) {
    display.clearDisplay();
    printCentered("AHT30 Not Found!", 20, 1);
    printCentered("Check wiring.", 35, 1);
    display.display();
    while (true);
  }

  Serial.println("Setup complete!");
}

// -----------------------------------------------
//   LOOP
// -----------------------------------------------
void loop() {
  readSensor();

  if (isPressed(BTN_NEXT, lastState1, lastDebounce1)) {
    currentScreen = (currentScreen + 1) % TOTAL_SCREENS;
    Serial.print("Screen: "); Serial.println(currentScreen);
  }

  if (isPressed(BTN_UNIT, lastState2, lastDebounce2)) {
    useFahrenheit = !useFahrenheit;
    Serial.println(useFahrenheit ? "Fahrenheit" : "Celsius");
  }

  switch (currentScreen) {
    case 0: showTemperature(); break;
    case 1: showHumidity();    break;
    case 2: showHeatIndex();   break;
    case 3: showStatus();      break;
    case 4: showMinMax();      break;
  }

  delay(100);
}

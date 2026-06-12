#define BLYNK_TEMPLATE_ID "TMPL3auzvYCuz"
#define BLYNK_TEMPLATE_NAME "Gas Leakage Detector"
#define BLYNK_AUTH_TOKEN "cpAM5bBuIhmnjFXg4Myod2qvMMKG4LIq"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define GREEN_LED 26
#define RED_LED 25
#define BUZZER 27
#define GAS_SENSOR 35

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool alertSent = false;

void setup() {

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(GAS_SENSOR, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Gas Detector");

  delay(2000);
  lcd.clear();
}

void loop() {

  Blynk.run();

  int gasValue = analogRead(GAS_SENSOR);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Blynk.virtualWrite(V0, gasValue);

  lcd.setCursor(0,0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print("     ");

  if(gasValue > 2000) {

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    tone(BUZZER, 1000);

    lcd.setCursor(0,1);
    lcd.print("GAS LEAK !!!  ");

    if(!alertSent) {
      Blynk.logEvent("gas_alert",
                     "Gas leakage detected!");
      alertSent = true;
    }

  }
  else {

    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

    noTone(BUZZER);

    lcd.setCursor(0,1);
    lcd.print("SAFE          ");

    alertSent = false;
  }

  delay(1000);
}
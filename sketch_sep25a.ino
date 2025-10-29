#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Define the pins for the hardware components
#define DHTPIN 7        // Digital pin for the DHT11 sensor data pin
#define BUZZER_PIN 8    // Digital pin for the buzzer's I/O pin
#define MQ2_PIN A0      // Analog pin for the MQ2 sensor's A0 pin

// Define thresholds for the alarm
// Adjust these values based on your testing and environment
const int TEMP_THRESHOLD = 30;  // Temperature in Celsius
const int SMOKE_THRESHOLD = 300; // Analog smoke value (0-1023)

// Initialize the LCD with its I2C address and dimensions (16 columns, 2 rows).
// A common I2C address is 0x27, but if it doesn't work, try 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize the DHT11 sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// A variable to store the state of the alarm
bool alarmActive = false;

void setup() {
  // Start the serial communication for debugging
  Serial.begin(9600);

  // Set up the LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.clear();
  lcd.print("Fire Detection");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");

  // Initialize the DHT sensor
  dht.begin();

  // Set the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
}

void loop() {
  // Read sensor values
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  int smokeValue = analogRead(MQ2_PIN);     // Read smoke level from MQ2

  // Check if sensor readings are valid
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.print("DHT Sensor Error");
    lcd.setCursor(0, 1);
    lcd.print("Check connections");
    delay(2000);
  } else {
    // Check if the alarm conditions are met
    if (temperature > TEMP_THRESHOLD || smokeValue > SMOKE_THRESHOLD) {
      // Alarm condition met
      alarmActive = true;
    } else {
      // Alarm condition not met
      alarmActive = false;
    }

    // Update the display based on the alarm state
    if (alarmActive) {
      // Display the alarm message
      Serial.println("!!! FIRE ALARM !!!");
      lcd.clear();
      lcd.print("Fire Detected!");
      lcd.setCursor(0, 1);
      lcd.print("T:");
      lcd.print(temperature);
      lcd.print(" S:");
      lcd.print(smokeValue);
      
      // Flash the buzzer on and off to create a pulsing sound
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);

    } else {
      // Display normal status
      Serial.println("System Normal");
      lcd.clear();
      lcd.print("Temp:");
      lcd.print(temperature);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Smoke:");
      lcd.print(smokeValue);
      digitalWrite(BUZZER_PIN, LOW); // Keep the buzzer off
    }
  }

  // Delay for a short period before the next loop iteration
  delay(1000);
}
#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#define MOISTURE_PIN A0          
#define METAL_PIN 2              
#define TRIG_PIN 3               
#define ECHO_PIN 4               
#define MOISTURE_THRESHOLD 700   
#define DISTANCE_THRESHOLD 5    
Servo servo1;   
Servo servo2;  
LiquidCrystal_I2C lcd(0x27, 16, 2);  
int metalCount = 0; 
int wetCount = 0; 
int dryCount = 0; 
void setup() { 
Serial.begin(9600);  
servo1.attach(9); 
servo2.attach(10); 
pinMode(MOISTURE_PIN, INPUT); 
pinMode(METAL_PIN, INPUT_PULLUP); 
pinMode(TRIG_PIN, OUTPUT); 
pinMode(ECHO_PIN, INPUT);  
lcd.init(); 
lcd.backlight(); 
lcd.setCursor(0, 0); 
lcd.print("Waste Sorting"); 
delay(2000); 
Serial.println("Setup Complete. Waiting for waste detection...");
}
void loop() { 
  long duration = measureDistance(); 
 
  float distance = duration * 0.034 / 2; 
 
  int moistureValue = analogRead(MOISTURE_PIN); 
  bool isMetalDetected = !digitalRead(METAL_PIN); 
 
  String wasteType = ""; 
 
  Serial.print("Distance: "); 
  Serial.print(distance); 
  Serial.println(" cm"); 
 
  Serial.print("Moisture Value: "); 
  Serial.println(moistureValue); 
 
  if (distance < DISTANCE_THRESHOLD) {  
    lcd.setCursor(0, 0); 
    lcd.print("Waste Detected!  "); 
    delay(2000); 
 
    if (isMetalDetected) { 
      servo1.write(180);  
      lcd.setCursor(0, 0); 
      lcd.print("Metal Detected!  "); 
      wasteType = "Metal"; 
      metalCount++;   
      Serial.println("Metal Waste Detected!"); 
      delay(1000); 
      servo2.write(0);  
    } else if (moistureValue < MOISTURE_THRESHOLD) { 
      servo1.write(90);  
      lcd.setCursor(0, 0); 
      lcd.print("Wet Waste!       "); 
      wasteType = "Wet"; 
      wetCount++;   
      Serial.println("Wet Waste Detected!"); 
      delay(1000); 
      servo2.write(0);  
    } else { 
      servo1.write(0);  
      lcd.setCursor(0, 0); 
      lcd.print("Dry Waste!       "); 
      wasteType = "Dry"; 
      dryCount++;  
 Serial.println("Dry Waste Detected!"); 
      delay(1000); 
      servo2.write(0);  
 
    } 
  }  
 
else { 
    lcd.setCursor(0, 0); 
    lcd.print("No Waste Found   "); 
    wasteType = "None"; 
    servo1.write(0);  // Reset servo1 position 
    servo2.write(90);  // Reset servo2 position 
    Serial.println("No Waste Detected"); 
  } 
 
  Serial.println("---- Waste Status ----"); 
  Serial.print("Waste Type: "); 
  Serial.println(wasteType); 
  Serial.print("Metal Waste Count: "); 
  Serial.println(metalCount); 
  Serial.print("Wet Waste Count: "); 
  Serial.println(wetCount); 
  Serial.print("Dry Waste Count: "); 
  Serial.println(dryCount); 
 
  Serial.print("Data: "); 
  Serial.print(wasteType); 
  Serial.print(","); 
  Serial.print(metalCount); 
  Serial.print(","); 
  Serial.print(wetCount); 
  Serial.print(","); 
  Serial.println(dryCount); 
 
  delay(1000);  
} 
 
long measureDistance() { 
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIG_PIN, LOW); 
  return pulseIn(ECHO_PIN, HIGH);
}

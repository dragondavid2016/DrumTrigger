// Drum Trigger Test
// rev 0.3, 1/2/2016, D. Kent
//   Changed LED output to be inverted for RGB strip mini amplifier drive
//     added support for 3 colors.  Added LED test in setup
// rev 0.2, 10/6/2015, Kavid Kent
//   Added min on time pot, added re-trigger holdoff pot
//   NOT TESTED!
// rev 0.1, 9/5/2015, David Kent
//


#define minOnTime 10
#define thresholdSampleTime 250

int piezoPin = 0;
int outputPin = 4;
int thresholdPin = A1;
int onTimePin = A2;
int holdoffTimePin = A3;
int LEDoutPin = 13;
int LEDoutPinGRN = 12;
int LEDoutPinBLU = 11;
int piezo;
int threshold, onTime, holdoffTime;
int holdOff;
unsigned int ANALOG_MAX;
unsigned long time1;
//unsigned long thresholdSampleTime = 250;  // milliseconds
unsigned long time2, time3;
unsigned int ANALOG_REF;

void setup() {
  // put your setup code here, to run once:
  pinMode(outputPin, OUTPUT);
  pinMode(LEDoutPin, OUTPUT);

  ANALOG_MAX = 10;
  ANALOG_REF = (unsigned int) 255*3.3/5.0;
  
//  Serial.begin(19200);
  threshold = analogRead(thresholdPin);
  holdoffTime = analogRead(holdoffTimePin);  //max 1024 milliseconds (1.024 second)
  onTime = analogRead(onTimePin);  //max 1024 milliseconds (1.024 second)
  time1 = onTime + 1;
  time2 = millis();
  time3 = 0;
  holdOff = 1;
//  Serial.println(threshold);

  //test the LED strip colors
  analogWrite(LEDoutPin, 255);
  analogWrite(LEDoutPinGRN, 255);
  analogWrite(LEDoutPinBLU, 255);
  delay(100);
  analogWrite(LEDoutPinGRN, 0);
  delay(500);
  analogWrite(LEDoutPinGRN, 255);
  analogWrite(LEDoutPin, 0);
  delay(500);
  analogWrite(LEDoutPin, 255);
  analogWrite(LEDoutPinBLU, 0);
  delay(500);
  analogWrite(LEDoutPinBLU, 255);
  delay(500);
  analogWrite(LEDoutPin, 255);
  analogWrite(LEDoutPinGRN, 255);
  analogWrite(LEDoutPinBLU, 255);
  delay(100);
//  digitalWrite(outputPin, HIGH);
//  delay(1000);
//  digitalWrite(outputPin, LOW);  
}

void loop() {
//  Serial.print(analogRead(piezoPin));
//  Serial.print(", ");
//  Serial.print(analogRead(thresholdPin));
//  Serial.print(", ");
//  Serial.println(analogRead(onTimePin));

  // sample pot values periodically
  // will it take too long to read 3 analog inputs?  If so do we want to spread them out?
  if ( (millis() - time2) > thresholdSampleTime ) {
    threshold = analogRead(thresholdPin);
    onTime = map(analogRead(onTimePin), 0, 1023, minOnTime, 255);
    holdoffTime = analogRead(holdoffTimePin);
    time2 = millis();
  }

  piezo = analogRead(piezoPin);

  if ( ( piezo >= threshold ) && ( (millis() - time3) > holdoffTime) ){
    digitalWrite(outputPin, HIGH);
    analogWrite(LEDoutPin, 0 );
    time1 = millis();  //reset on time
    if (piezo > ANALOG_MAX) ANALOG_MAX = piezo;  //AGC (peak hold) for LED PWM output
    holdOff = 1;
  }

  if ( (millis() - time1) >= onTime ) {
    digitalWrite(outputPin, LOW);
    analogWrite(LEDoutPin, 255 );
    if (holdOff == 1) {
      time3 = millis();  //reset holdoff time
      holdOff = 0;
    }
  }
  
 // analogWrite(LEDoutPin, map(piezo, 0, ANALOG_MAX, 255, 0) );
/*  if (map(piezo, 0, ANALOG_MAX, 255, 0) > 128) {
    analogWrite(LEDoutPinGRN, 255);
  } else analogWrite(LEDoutPinGRN, map(piezo, 0, ANALOG_MAX, 255, 0) );
*/
  analogWrite(LEDoutPinGRN, map(piezo, 0, ANALOG_REF, 255, 0) );
  analogWrite(LEDoutPinBLU, map(piezo, 0, ANALOG_REF, 255, 0) );
}

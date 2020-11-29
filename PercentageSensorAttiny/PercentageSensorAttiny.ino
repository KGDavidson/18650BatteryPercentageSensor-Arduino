#include <SoftwareSerial.h>

////////// Change these as needed //////////
const int analogSensorPin = A2;
const int sensorRepeats = 10;
const int numOf18650Cells = 1;


const bool attinyInputVoltageIsKnown = true;
const float attinyInputVoltage = 5.0;
////////// Change these as needed //////////

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // Read Voltage
  float totalVoltage = readVoltage();

  // Get Voltage of Singular 18650 Cell
  float singleCellVoltage = totalVoltageoltage / numOf18650Cells

  // Calculate Battery Percentage
  float percentage = calculatePercentage();

  // Print Values
  Serial.println((String)"Voltage: " + totalVoltage + "V");
  Serial.println((String)"Percentage: " + percentage + "%");
}

float readVoltage() {
  float totalAvgVoltage = 0.00;
  float inputVoltage = attinyInputVoltage;
  if (!attinyInputVoltageIsKnown) {
    inputVoltage = readInputVoltage();
  }

  // Add up Voltage over Repeats
  for (int i = 0; i < sensorRepeats; i++){
    int sensorValue = analogRead(A2);
    totalAvgVoltage += sensorValue * (inputVoltage / 1024.0);
  }

  // Get Average Voltage
  avgVoltage = totalAvgVoltage / sensorRepeats;
  
  return avgVoltage;
}

float calculatePercentage(float voltage) {
  // Map of State of Charge Curve
  if (voltage >= 3.44 && voltage <= 3.62){
      return 100.0 * ((107.0 / 90.0 * voltage) - (4601.0 / 1125.0));
  }
  if (voltage >= 3.62 && voltage <= 3.706){
      return 100.0 * ((259.0 / 86.0 * voltage) - (114897.0 / 10750.0));
  }
  if (voltage >= 3.706 && voltage <= 3.932){
      return 100.0 * ((142.0 / 113.0 * voltage) - (472803.0 / 113000.0));
  }
  if (voltage >= 3.932 && voltage <= 4.063){
      return 100.0 * ((127.0 / 131.0 * voltage) - (400197.0 / 131000.0));
  }
  if (voltage >= 4.063 && voltage <= 4.17){
      return 100.0 * ((116.0 / 137.0 * voltage) - (1751.0 / 685.0));
  }
  
  // If above Max Voltage of 18650
  if (voltage > 4.17) {
      return 100;
  }

  // If below Min Voltage of 18650
  return 0.0f;
}

float readInputVoltage() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
  
  // Wait for Vref to settle
  delay(2);
  // Start conversion  
  ADCSRA |= _BV(ADSC);
  // measuring
  while (bit_is_set(ADCSRA,ADSC));
  
  // must read ADCL first - it then locks ADCH 
  uint8_t low  = ADCL;  
  // unlocks both
  uint8_t high = ADCH; 

  long result = (high<<8) | low;
  
  // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  result = 1125300L / result;
  
  // Vcc in millivolts
  return (String(result).substring(0, 3).toFloat() / 100.00);
}

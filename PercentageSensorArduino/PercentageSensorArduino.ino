#include <SoftwareSerial.h>

////////// Change these as needed //////////
const int analogSensorPin = A2;
const int sensorRepeats = 10;
int numOf18650Cells = 1;

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

  // Add up Voltage over Repeats
  for (int i = 0; i < sensorRepeats; i++){
    int sensorValue = analogRead(A2);
    totalAvgVoltage += sensorValue * (5.0 / 1024.0);
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

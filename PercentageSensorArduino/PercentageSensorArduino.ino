#include <SoftwareSerial.h>

///// Change these as needed /////
const int analogSensorPin = A2;
const int sensorRepeats = 10;
int numOf18650Cells = 1;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  float voltage = readVoltage();
  float percentage = calculatePercentage(voltage / numOf18650Cells);
  Serial.println((String)"Voltage: " + voltage + "V");
  Serial.println((String)"Percentage: " + percentage + "%");
}

float readVoltage() {
  float totalAvgVoltage = 0.00;
  for (int i = 0; i < sensorRepeats; i++){
    int sensorValue = analogRead(A2);
    totalAvgVoltage += sensorValue * (5.0 / 1024.0);
  }
  avgVoltage = totalAvgVoltage / sensorRepeats;
  return avgVoltage;
}

float calculatePercentage(float voltage) {
  
}

/*
 * Example for testing the RS-485 communication on ESP32-S3
 * Written by Phan Khac Hai 20231106- https://khphan.com/ 
 * refer to the RTU examples of modbus-esp8266 Library: https://github.com/emelianov/modbus-esp8266/tree/master/examples/RTU
 * & question discussion on the Internet: https://forum.allaboutcircuits.com/threads/esp32-rtu-modbus-simple-message-transmit-and-receive.173595/
 * SLAVE: it reads data from sensor and sends data back to master upon the master request
 */
 
#include <ModbusRTU.h>
#define RXD1 17
#define TXD1 18
#define SLAVE_ID 1 // Modbus Slave ID
#define REGN 1

const int analogPin = A0;

ModbusRTU mb;

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  Serial1.begin(9600,SERIAL_8N1, RXD1, TXD1);  // Start the Serial1 communication
  mb.begin(&Serial1);
  
  mb.slave(SLAVE_ID); // //Initializng modbus slave device with ID 1
  mb.addHreg(REGN); // add the register with address 1
  mb.Hreg(REGN, 100); // Exactly as was wtitten line above assigns value 100 to local HoldingReg #REGN. It's only for testing purpose
}

void loop() {
  mb.task(); // Process incoming Modbus requests

  // Read the analog value from the analog pin
  uint16_t sensorValue = analogRead(analogPin);

  // Update the holding register with the sensor value
  mb.Hreg(REGN, sensorValue); // Store the value in holding register 1
  delay(100); // Add a small delay for stability
  // It's strongly recommended to run .task() multiple times within timeout period (that is 1 second by default)
  
  yield();
  //Serial.println(sensorValue);
}

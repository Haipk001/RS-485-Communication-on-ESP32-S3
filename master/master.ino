/*
 * Example for testing the RS-485 communication on ESP32-S3
 * Written by Phan Khac Hai 20231106- https://khphan.com/ 
 * refer to the RTU examples of modbus-esp8266 Library: https://github.com/emelianov/modbus-esp8266/tree/master/examples/RTU
 * & question discussion on the Internet: https://forum.allaboutcircuits.com/threads/esp32-rtu-modbus-simple-message-transmit-and-receive.173595/
 * MASTER: it requests data from slave and prints the data on serial monitor
 */

#include <ModbusRTU.h>

#define SLAVE_ID 1
#define RXD1 17
#define TXD1 18
#define REGN 1
#define REG_COUNT 1

ModbusRTU mb;

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  mb.begin(&Serial1);
  mb.master();
}


void loop() {
  uint16_t res[REG_COUNT];
  if (!mb.slave()) {    // Check if no transaction in progress
    mb.readHreg(SLAVE_ID, REGN, res, REG_COUNT, cb); // Send Read Hreg from Modbus Server
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
    Serial.println(res[0]);
  }
  delay(1000);
}

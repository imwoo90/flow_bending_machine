#include <Controller.h>

void setup()
{
  flowBendingMachineController.setup();
}

void loop()
{
  flowBendingMachineController.loop();
}

// /*

//   RS485_HalfDuplex.pde - example using ModbusMaster library to communicate
//   with EPSolar LS2024B controller using a half-duplex RS485 transceiver.

//   This example is tested against an EPSolar LS2024B solar charge controller.
//   See here for protocol specs:
//   http://www.solar-elektro.cz/data/dokumenty/1733_modbus_protocol.pdf

//   Library:: ModbusMaster
//   Author:: Marius Kintel <marius at kintel dot net>

//   Copyright:: 2009-2016 Doc Walker

//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at

//       http://www.apache.org/licenses/LICENSE-2.0

//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

// */

// #include <ModbusMaster.h>
// #include <SoftwareSerial.h>
// #include <device/Keypad_4x3/Keypad_4x3.h>
// #include <Wire.h>

// /*!
//   We're using a MAX485-compatible RS485 Transceiver.
//   Rx/Tx is hooked up to the hardware serial port at 'Serial'.
//   The Data Enable and Receiver Enable pins are hooked up as follows:
// */
// #define MAX485_DE_RE 3
// #define MAX485_SERIAL ss_test
// #define MAX485_TX 4
// #define MAX485_RX 5

// // instantiate ModbusMaster object
// ModbusMaster node;
// SoftwareSerial ss_test(5, 4);

// void preTransmission()
// {
//   digitalWrite(MAX485_DE_RE, 1);
// }

// void postTransmission()
// {
//   digitalWrite(MAX485_DE_RE, 0);
// }

// void setup()
// {
//   printf("setup start\n");
//   pinMode(MAX485_DE_RE, OUTPUT);

//   // Init in receive mode
//   digitalWrite(MAX485_DE_RE, 0);

//   // // Change uart1 tx rx port
//   // Serial2.setTX(MAX485_TX);
//   // Serial2.setRX(MAX485_RX);
//   // Modbus communication runs at 9600 Band ,8 Data bits,None Parity,1 Stop Bit
//   MAX485_SERIAL.begin(9600);

//   // Modbus slave ID 1
//   node.begin(1, MAX485_SERIAL);

//   // Callbacks allow us to configure the RS485 transceiver correctly
//   node.preTransmission(preTransmission);
//   node.postTransmission(postTransmission);

//   // Set i2c0 interface
//   Wire.setClock(400000);
//   Wire.setSCL(17);
//   Wire.setSDA(16);
//   // subscribe keypad_4x3
//   Keypad_4x3* kp = Keypad_4x3::getInstance();
//   kp->subscribe([](const char key) {
//     printf("%c\n\r", key);
//   });
// }

// void loop()
// {
//   uint16_t toggle = 0x0300;
//   node.writeSingleRegister(0x5, toggle);
//   delay(1000);
// }

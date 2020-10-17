#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void controlSetup() {
  SerialBT.begin("ESP32Tetris");
}

void controlLoop() {
  if (SerialBT.available()) {
    char keyPress = (char)SerialBT.read();
    switch (keyPress) {
      case 'w':
        currentInput = ROTATE;
        break;
      case 'a':
        currentInput = LEFT;
        break;
      case 's':
        currentInput = DOWN;
        break;
      case 'd':
        currentInput = RIGHT;
        break;
    }
    Serial.println(currentInput);
  }
}

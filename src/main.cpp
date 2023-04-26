#include <Arduino.h>
#include "cmd_module.h"

//#include "RemotePult.h"
//#include "ActionTable.h"
#include "Utils.h"
#include "config.h"

//IRrecv irrecv(2); // указываем вывод, к которому подключен приемник
//RemotePult remotePult(2, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize):IRrecv(_recvpin) {
//RemotePult remotePult(IR_RECEIVER_PIN, TABLE(keyTable));

CommandHandler_t cmd;

void setup() {
  // put your setup code here, to run once:
  cmd_init(&cmd); // запуск интерпретатора командной строки
  //remotePult.enableIRIn(); // запускаем прием
}

void loop() {
  // put your main code here, to run repeatedly:
  cmd_process(&cmd);
  //remotePult.operate();
}

//-==================================================================





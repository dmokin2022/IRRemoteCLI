#include <Arduino.h>
#include "cmd_module.h"

#include "RemotePult.h"
#include "Utils.h"
#include "config.h"
//#include <IRremote.h>
//#include "IRremotePortable/IRremote.h"
//#include "key_table.h"

//IRrecv irrecv(IR_RECEIVER_PIN); // указываем вывод, к которому подключен приемник
//RemotePult remotePult(IR_RECEIVER_PIN, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize):IRrecv(_recvpin) {
//RemotePult remotePult(IR_RECEIVER_PIN, TABLE(key_table));
//RemotePult remotePult(IR_RECEIVER_PIN, &key_table[0], MAX_KEYS_COUNT);

CommandHandler_t cmd;
RemotePult *remotePult;

void setup() {
  cmd_init(&cmd); // запуск интерпретатора командной строки
  remotePult = new RemotePult(IR_RECEIVER_PIN, TABLE(key_table));
  // Запускаем прием
  remotePult->ir->enableIRIn();
  //remotePult.enableIRIn(); // запускаем прием
}

void loop() {
  // put your main code here, to run repeatedly:
  cmd_process(&cmd);
  remotePult->process();
}

//-==================================================================






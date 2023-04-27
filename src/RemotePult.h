#pragma once
#ifndef REMOTEPULT_H
#define REMOTEPULT_H

//#include <IRremote.h>
//#include "IRremotePortable/IRremote.h"
#include "IRremoteOld/IRremote.h"
#include "key_table.h"

enum keyStates {
    KEY_NO,
    KEY_PRESSED,
    KEY_AUTOREPEAT,
    KEY_RELEASED
};

#define NOT_FOUND -1

//class RemotePult: public IRrecv  {
class RemotePult  {    
    public:
        //RemotePult(int _recvpin, key_data_t* _key_table, uint8_t _rows_in_keytable):IRrecv(_recvpin) {
        RemotePult(int _recvpin, key_data_t* _key_table, uint8_t _rows_in_keytable);
        RemotePult (int recvpin);
    	RemotePult (int recvpin, int blinkpin);
        void process();

        IRrecv* ir;
    protected:
    private:
        //decode_results results; // ��������� ��� �������� ����������� ����� � ������
        
        key_data_t* key_table;    // указетель на таблицу кодов кнопок и обработчиков
        uint8_t rows_in_keytable;    // размер таблицы в строках
        keyStates state;  // состояние нажатия кнопки. Определяетя keyStates
        //decode_results results; // структура для хранения результатов приёма с пульта
        uint32_t newButtonCode;
        uint32_t lastButtonCode;  // код последней нажаток кнопки
        uint32_t lastButtonTime;  // время нажатия последней кнопки
        uint32_t timeEllapsed;      // время прошедшее с последней нажатой кнопки
        uint8_t buttonPositionInTable;

        uint8_t findPositionInTable(uint32_t _buttonCode);
        void interpretateKeyActionByTableSettings(keyStates state, uint8_t buttonPositionInTable);
        void onButtonPressed(uint8_t button);
        void onButtonReleased(uint8_t button);
        void onButtonAutorepeat(uint8_t button);
};
#endif // REMOTEPULT_H

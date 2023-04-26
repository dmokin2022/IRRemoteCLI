#ifndef REMOTEPULT_H
#define REMOTEPULT_H

#include <IRremote.h>

// Макросы для упрощённого составления таблицы функций нажатия кнопок
#define REACTION_ON_PRESSED(key)    {key, &on##key##Pressed, 0, 0}
#define REACTION_ON_RELEASED(key)   {key, 0, &on##key##Released, 0}
#define REACTION_ON_AUTOREPEAT(key) {key, 0, 0, &on##key##Autorepeat}

#define ON_PRESSED(key) void on##key##Pressed()
#define ON_RELEASED(key) void on##key##Released()
#define ON_AUTOREPEAT(key) void on##key##Autorepeat()

#define PTR_ON_PRESSED(key) &on##key##Pressed
#define PTR_ON_RELEASED(key) &on##key##Released
#define PTR_ON_AUTOREPEAT(key) &on##key##Autorepeat


enum keyStates {
    KEY_NO,
    KEY_PRESSED,
    KEY_AUTOREPEAT,
    KEY_RELEASED

};

#define NOT_FOUND -1


typedef struct _PultButtonItem {
    uint32_t buttonCode;    // код нажатой кнопки
    //keyStates keyState;
    void (*onButtonPressed)();  // указатель на функцию обработчик нажатой кнопки
    void (*onButtonAutorepeat)();
    void (*onButtonReleased)();
} PultButtonItem;


class RemotePult: public IRrecv  {
    public:
        PultButtonItem* pultButtonTable;    // указетель на таблицу кодов кнопок и обработчиков
        uint8_t pultButtonTableSize;    // размер таблицы в строках
        keyStates state;  // состояние нажатия кнопки. Определяетя keyStates

        decode_results results; // структура для хранения результатов приёма с пульта
        uint32_t newButtonCode;
        uint32_t lastButtonCode;  // код последней нажаток кнопки
        uint32_t lastButtonTime;  // время нажатия последней кнопки
        uint32_t timeEllapsed;      // время прошедшее с последней нажатой кнопки

        //uint32_t lastButtonCode;
        uint8_t buttonPositionInTable;

        RemotePult(int _recvpin, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize):IRrecv(_recvpin) {
            //IRrecv(_recvpin);   // вызов конструктора для родительского класса
            //irparams.recvpin = _recvpin;
            //irparams.blinkflag = 0;
            state = KEY_NO;
            lastButtonCode = -1;

            pultButtonTable = _pultButtonTable;
            pultButtonTableSize = _pultButtonTableSize;
        }


        RemotePult (int recvpin);
    	RemotePult (int recvpin, int blinkpin);
        //void traceDecodedIRData();

        //bool codeIsFindedInTable(uint32_t _buttonCode);
        uint8_t findPositionInTable(uint32_t _buttonCode);

        void process();
        //virtual ~RemotePult();
    protected:
    private:
};


#endif // REMOTEPULT_H

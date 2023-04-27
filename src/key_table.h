#pragma once
#include <Arduino.h>
#include "config.h"

// Режимы работы кнопок
enum {
    MODE_TOGGLE = 1,    // кнопка выдаёт 1 при нажатии и 0 при повторном нажатии
    MODE_HOLD,          // кнопка выдаёт 1 при удержании и 0 когда не нажата
    MODE_REPEAT,        // кнопка повторяет (генерирует) событие пока удерживается
};

// Формат хранения данных кнопки
typedef struct _key_data {
    uint8_t id; //Номер кнопки
    uint32_t code;  //Код нажатия
    char name[20];   //Название кнопки
    uint8_t mode;   //Режим нажатия (togle hold repeat)
    uint16_t pin;   //Номер ножки
		
    int16_t var_val;   //Связанная переменная
    int16_t var_step;  //Шаг изменения переменной
    int8_t var_ref;     // индекс связанной переменной в массиве кнопок
} key_data_t;


extern key_data_t key_table[MAX_KEYS_COUNT];

void keytable_output_item(uint8_t i);

// Вывод всей таблицы
void keytable_output_all();

void keytable_save_to_EEPROM();
void keytable_load_from_EEPROM();
void keytable_reset();

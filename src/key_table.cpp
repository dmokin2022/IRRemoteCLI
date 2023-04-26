#include <Arduino.h>
#include "key_table.h"
#include "Utils.h"
#include <EEPROM.h>

key_data_t key_table[MAX_KEYS_COUNT];

void keytable_output_item(uint8_t i) {
    PRINT("id = "); PRINT(key_table[i].id); PRINT(" ");
    PRINT("code = "); PRINT(key_table[i].code); PRINT(" ");
    PRINT("name = "); PRINT(key_table[i].name); PRINT(" ");
    PRINT("mode = "); PRINT(key_table[i].mode); PRINT(" ");
    PRINT("pin = "); PRINT(key_table[i].pin); PRINT(" ");
    PRINT("val = "); PRINT(key_table[i].var_val); PRINT(" ");
    PRINT("step = "); PRINT(key_table[i].var_step); PRINT(" ");
    PRINT("ref = "); PRINT(key_table[i].var_ref); PRINT(" ");
    PRINTLN();
}

// Вывод всей таблицы (все параметры всех кнопок)
void keytable_output_all() {
    //for (int i = 1; i < (sizeof(key_table)/sizeof(key_table[0])); i++)
    FOR_EACH(key_table, i) {
        keytable_output_item(i);
    }
}

// Установка в строке таблицы значений по умолчанию
void keytable_set_default(int item) {
    //key_data_t data = {
    key_data_t data;
        data.id = item,
        data.code = 0x00000000,
        strcpy(&data.name[0], "button__"),
        //data.name = "button__",
        data.mode = MODE_TOGGLE,
        data.pin = 0,
        data.var_val = 0xff,
        data.var_step = 10,
        data.var_ref = 0;
    //};

    key_table[item] = data;
}

// Установка во всей таблице значений по умолчанию
void keytable_fill_by_default() {
    FOR_EACH(key_table, i) {
        keytable_set_default(i);
    }
}

void keytable_save_to_EEPROM() {
    EEPROM.put(0, key_table);
}

void keytable_load_from_EEPROM() {
    EEPROM.get(0, key_table);
}

void keytable_reset() {
    memset(&key_table, 0, sizeof(key_table));
}
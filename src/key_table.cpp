#include <Arduino.h>
#include "key_table.h"
#include "Utils.h"
#include <EEPROM.h>

key_data_t key_table[MAX_KEYS_COUNT];
uint8_t currentCapturedKeyIndex;    // индекс текущей позиции в таблице для записи кодов клавишь

void keytable_output_item(uint8_t i) {
    PRINT("id = "); PRINT(key_table[i].id); PRINT(" ");
    //PRINT("code = "); PRINT(key_table[i].code); PRINT(" ");
    PRINT("code = "); keytable_output_hex(key_table[i].code); PRINT(" ");
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
    //EEPROM.write(0, currentCapturedKeyIndex);
    EEPROM.put(0, key_table);
}

void keytable_load_from_EEPROM() {
    EEPROM.get(0, key_table);
}

void keytable_reset() {
    memset(&key_table, 0, sizeof(key_table));
    currentCapturedKeyIndex = 1;
}

void keytable_init() {
    keytable_load_from_EEPROM();
    currentCapturedKeyIndex = 1;
}

void keytable_add_key(uint32_t code) {
    // Запись кода кнопки в таблицу (остальные параметры по умолчанию)
    if (currentCapturedKeyIndex < MAX_KEYS_COUNT) {
        keytable_set_default(currentCapturedKeyIndex);
        key_table[currentCapturedKeyIndex].code = code;
        keytable_generate_name_for_key(currentCapturedKeyIndex);
       
        currentCapturedKeyIndex++;
        PRINT("Key "); PRINT(code); PRINTLN(" is added to table");
    } else {
        PRINTLN("Key table is full. Delete any old key to add new one.");
    }
}

void keytable_generate_name_for_key(uint8_t number) {
    char buff[MAX_SIMBOLS_IN_WORD];
    sprintf(buff, "BUT_%u", number);
    strncpy(key_table[currentCapturedKeyIndex].name, buff, MAX_SIMBOLS_IN_WORD);
}

void keytable_output_hex(uint32_t code) {
    Serial.println( code, HEX );
}
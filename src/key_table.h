#pragma once
#include <Arduino.h>
#include "config.h"
#include "key_data.h"


extern key_data_t key_table[MAX_KEYS_COUNT];

void keytable_output_item(uint8_t i);

void keytable_output_all(); // Вывод всей таблицы
void keytable_set_default(int item);
void keytable_save_to_EEPROM();
void keytable_load_from_EEPROM();
void keytable_reset();
void keytable_init();
void keytable_add_key(uint32_t code);
void keytable_output_hex(uint32_t code);
void keytable_generate_name_for_key(uint8_t number);
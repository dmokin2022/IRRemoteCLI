#include <Arduino.h>
#include "config.h"

// Режимы работы кнопок
enum {
    MODE_TOGGLE = 1,
    MODE_HOLD,
    MODE_REPEAT,
};

// Формат хранения данных кнопки
typedef struct _key_data {
    uint8_t id; //Номер кнопки
    uint32_t code;  //Код нажатия
    char name[20];   //Название кнопки
    uint8_t mode;   //Режим нажатия (togle hold repeat)
    uint16_t pin;   //Номер ножки
		
    uint16_t var_val;   //Связанная переменная
    uint16_t var_step;  //Шаг изменения переменной
    uint8_t var_ref;     // индекс связанной переменной в массиве кнопок
} key_data_t;


extern key_data_t key_table[MAX_KEYS_COUNT];

void keytable_output_item(uint8_t i);

// Вывод всей таблицы
void keytable_output_all();
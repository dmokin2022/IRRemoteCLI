#include "cmd_interpretator.h"
#include "cmd_var_access.h"
#include "Utils.h"
#include "config.h"
#include <Arduino.h>
#include <stdio.h>
#include "key_table.h"


// Формат команды
enum {
	COMMAND,
	BUTTON,
	PARAMETER,
	VALUE,
};

// Макрос для упрощения сравнения слов
#define EQUAL(i, str) (strcmp(cmd->words[i], str) == 0)
#define IS_EMPTY(i) (cmd->words[i][0] == 0)
#define WORD(i) cmd->words[i]

// Интерпретатор команд
void cmd_interpretate(CommandHandler_t* cmd)
{
	// Вывод отладочных данных
	if (DEBUG) {
		FOR_(4, i) {
			char buff[50];
			sprintf(buff, "%d: %s - %d", i, cmd->words[i], strlen(cmd->words[i]));
			Serial.println(buff);
		}
	}

	bool command_is_executed = true;
	int button = atoi(cmd->words[BUTTON]);	// Определяем номер кнопки 
	int value = atoi(cmd->words[VALUE]);	// ... значение параметра

	//-----------------------------------------------------------------
	if (EQUAL(COMMAND, "set")) {
		if (EQUAL(PARAMETER, "id")) { key_table[button].id = value; } else
		if (EQUAL(PARAMETER, "code")) { key_table[button].code = value; } else
		if (EQUAL(PARAMETER, "pin")) { key_table[button].pin = value; } else
		if (EQUAL(PARAMETER, "val")) { key_table[button].var_val = value; } else
		if (EQUAL(PARAMETER, "step")) { key_table[button].var_step = value; } else
		if (EQUAL(PARAMETER, "ref")) { key_table[button].var_ref = value; } else
		if (EQUAL(PARAMETER, "mode")) { 
			key_table[button].mode = value;
			if (value == 0) {
				if (EQUAL(VALUE, "toggle")) { key_table[button].mode = MODE_TOGGLE; } else
				if (EQUAL(VALUE, "hold")) { key_table[button].mode = MODE_HOLD; } else
				if (EQUAL(VALUE, "repeat")) { key_table[button].mode = MODE_REPEAT; }
			}
		} else
		if (EQUAL(PARAMETER, "name")) {
			strncpy(key_table[button].name, cmd->words[VALUE], MAX_SIMBOLS_IN_WORD);
			// Проверка длины названия кнопки
			// if (strlen(cmd->words[VALUE]) <= MAX_SIMBOLS_IN_WORD) {
			// 	strcpy(key_table[button].name, cmd->words[VALUE]);
			// }
		}
	} else 
	if (EQUAL(COMMAND, "get")) {
		// PRINTLN("найдена команда get");
		// PRINTLN(button);
		// Если код кнопки задан (!= 0)
		if (button) {
			if (EQUAL(PARAMETER, "id")) { PRINTLN(key_table[button].id);  }
			if (EQUAL(PARAMETER, "code")) { PRINTLN(key_table[button].code);  }
			if (EQUAL(PARAMETER, "pin")) { PRINTLN(key_table[button].pin);  }
			if (EQUAL(PARAMETER, "val")) { PRINTLN(key_table[button].var_val);  }
			if (EQUAL(PARAMETER, "step")) { PRINTLN(key_table[button].var_step);  }
			if (EQUAL(PARAMETER, "ref")) { PRINTLN(key_table[button].var_ref);  }
			if (EQUAL(PARAMETER, "name")) { PRINTLN(key_table[button].name);  }
			if (EQUAL(PARAMETER, "mode")) { 
				switch (key_table[button].mode) {
					MODE_TOGGLE: PRINTLN("toggle"); break;
					MODE_REPEAT: PRINTLN("repeat"); break;
					MODE_HOLD: PRINTLN("hold"); break;
				};
			}
		} else {
			// Команда get без параметров (button = 0)
			keytable_output_all();
		}
	} else
	if (EQUAL(COMMAND, "capture") || EQUAL(COMMAND, "scan")) {

	} else 
	if (EQUAL(COMMAND, "save")) {
		PRINT("Table size in bytes: ");
		PRINTLN(sizeof(key_table));
		keytable_save_to_EEPROM();
		PRINTLN("Table is saved");
 
	} else 
	if (EQUAL(COMMAND, "load")) {
		keytable_load_from_EEPROM();
		PRINTLN("Table is loaded");
	} else
	if (EQUAL(COMMAND, "reset")) {
		keytable_reset();
		PRINTLN("Table is cleared");
	} else 
	
	{
		// Если ни одна команда не выполнена
		command_is_executed = false;	
	}


	if (command_is_executed) {
		cmd_outputMessage(cmd, "Ok");
	} else {
		cmd_outputMessage(cmd, "Unknown command");
	}
}
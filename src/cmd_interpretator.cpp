#include <Arduino.h>
#include <stdio.h>

#include "cmd_interpretator.h"
#include "Utils.h"
#include "config.h"
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

void cmd_show_debug_info(CommandHandler_t* cmd) {
		FOR_(4, i) {
			char buff[50];
			sprintf(buff, "%d: %s - %d", i, cmd->words[i], strlen(cmd->words[i]));
			Serial.println(buff);
		}
}

// Интерпретатор команд
void cmd_interpretate(CommandHandler_t* cmd)
{
	// Вывод отладочных данных
	if (CMD_DEBUG) cmd_show_debug_info(cmd);

	bool command_is_executed = true;
	int button = atoi(cmd->words[BUTTON]);	// Определяем номер кнопки 
	int value = atoi(cmd->words[VALUE]);	// ... значение параметра

	//-----------------------------------------------------------------
	if (EQUAL(COMMAND, "set")) {
		key_table[button].id = button;
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
		}
	} else 
	if (EQUAL(COMMAND, "get")) {
		// PRINTLN("найдена команда get");
		// PRINTLN(button);
		// Если код кнопки задан (!= 0)
		if (button) {
			if (EQUAL(PARAMETER, "id")) { PRINTLN(key_table[button].id);  } else
			if (EQUAL(PARAMETER, "code")) { PRINTLN(key_table[button].code);  } else
			if (EQUAL(PARAMETER, "pin")) { PRINTLN(key_table[button].pin);  } else
			if (EQUAL(PARAMETER, "val")) { PRINTLN(key_table[button].var_val);  } else
			if (EQUAL(PARAMETER, "step")) { PRINTLN(key_table[button].var_step);  } else
			if (EQUAL(PARAMETER, "ref")) { PRINTLN(key_table[button].var_ref);  } else
			if (EQUAL(PARAMETER, "name")) { PRINTLN(key_table[button].name);  } else
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
		remotePult->setMode(RMODE_CAPTURE);
	} else 
	if (EQUAL(COMMAND, "gen") || EQUAL(COMMAND, "start")) {
		remotePult->setMode(RMODE_GENERATE);
	} else 
	if (EQUAL(COMMAND, "save")) {
		PRINT("Table size in bytes: ");
		PRINTLN(sizeof(key_table));
		keytable_save_to_EEPROM();
		PRINTLN("Table is saved to EEPROM");
 
	} else 
	if (EQUAL(COMMAND, "load")) {
		keytable_load_from_EEPROM();
		PRINTLN("Table is loaded from EEPROM");
	} else
	if (EQUAL(COMMAND, "reset")) {
		keytable_reset();
		PRINTLN("Table is cleared in RAM");
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

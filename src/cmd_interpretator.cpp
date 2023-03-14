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

	if (DEBUG) {
		FOR_(4, i) {
			char buff[50];
			sprintf(buff, "%d: %s - %d", i, cmd->words[i], strlen(cmd->words[i]));
			Serial.println(buff);
		}
	}
	
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
				if (EQUAL(VALUE, "toggle")) {  } else
				if (EQUAL(VALUE, "hold")) {  } else
				if (EQUAL(VALUE, "repeat")) {  }
			}
		}
	} else 
	if (EQUAL(COMMAND, "get")) {
		if (button) {
			if (EQUAL(PARAMETER, "id")) { PRINTLN(key_table[button].id);  }
			if (EQUAL(PARAMETER, "code")) { PRINTLN(key_table[button].code);  }
			if (EQUAL(PARAMETER, "pin")) { PRINTLN(key_table[button].pin);  }
			if (EQUAL(PARAMETER, "val")) { PRINTLN(key_table[button].var_val);  }
			if (EQUAL(PARAMETER, "step")) { PRINTLN(key_table[button].var_step);  }
			if (EQUAL(PARAMETER, "ref")) { PRINTLN(key_table[button].var_ref);  }
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

	}

	


	cmd_outputMessage(cmd, "Unknown command");
}

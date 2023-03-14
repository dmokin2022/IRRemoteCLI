
#include "KeyReaction.h"
#include "Utils.h"

#include "key_table.h"

// Поиск совпадения кода нажатой кнопки со значениями в таблице
uint8_t KeyReaction::findPositionInTable(uint32_t buttonCode) {
    uint8_t i;

    // Проход по всей таблице в поиске совпадающего кода
    for (i = 0; i < pultButtonTableSize; i++) {
        if (buttonCode == pultButtonTable[i].code) {
            break;
        }
    }

    // Если код обнаружен в таблице
    if (i < pultButtonTableSize) return i;
    else return NOT_FOUND;

}

void KeyReaction::outputKey(uint16_t keyNum) {
    // Если кнопка не является ссылочной на другое значение
    if (!pultButtonTable[keyNum].var_ref) {
        // Если значение для вывода максимально (логическая 1)
        if (pultButtonTable[keyNum].var_val >= 0xff) {
            digitalWrite(pultButtonTable[keyNum].pin, pultButtonTable[keyNum].var_val);
        } else
        if (pultButtonTable[keyNum].var_val == 0) {
            digitalWrite(pultButtonTable[keyNum].pin, pultButtonTable[keyNum].var_val);
        } else
            // В интервале значений от 1 до 254 аналоговый вывод (ШИМ - PWM)
            analogWrite(pultButtonTable[keyNum].pin, pultButtonTable[keyNum].var_val);
            




    } else {
    
    
    }

}

void KeyReaction::process() {
    uint8_t i;
    // Если таблица соответствия кодов определена
    if (pultButtonTable != 0) {

        // Если данные пришли
        if (decode()) {

            newButtonCode = decodedIRData.decodedRawData;

            // Сохранение времени последнего прихода данных
            lastButtonTime = millis();

            if (state == KEY_NO) {
                buttonPositionInTable = findPositionInTable(newButtonCode);
                if (buttonPositionInTable == NOT_FOUND) {
                }
                else {
                    state = KEY_PRESSED;
                    // debug(state);
                    //CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonPressed);
                    outputKey(buttonPositionInTable);

                    lastButtonCode = newButtonCode;
                }
            }
            else if (state == KEY_PRESSED)
            {
                if ((newButtonCode == 0x0))
                {
                    state = KEY_AUTOREPEAT;
                    // debug(state);

                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);
                }
            }
            else if (state == KEY_AUTOREPEAT)
            {
                // debug(state);
                // debug(buttonPositionInTable);
                CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonAutorepeat);
            }

            resume(); // принимаем следующую команду
            // Если данные не приходили
        }
        else
        {
            if ((state == KEY_PRESSED) || (state == KEY_AUTOREPEAT))
            {
                timeEllapsed = millis() - lastButtonTime;
                if (timeEllapsed > 200)
                {
                    state = KEY_RELEASED;
                    // debug(state);
                    // debug(timeEllapsed);
                    CALL_IF_NOT_NULL(pultButtonTable[buttonPositionInTable].onButtonReleased);
                    state = KEY_NO;
                    // debug(state);
                }
            }
        }
    }
}


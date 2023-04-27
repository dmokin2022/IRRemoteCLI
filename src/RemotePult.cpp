
#include "RemotePult.h"
#include "Utils.h"
#include "pins_reaction.h"


RemotePult::RemotePult(int _recvpin, key_data_t* _key_table, uint8_t _rows_in_keytable) {            
            ir = new IRrecv(_recvpin);
            state = KEY_NO;
            lastButtonCode = -1;
            key_table = _key_table;
            //rows_in_keytable = size_of_keytable/sizeof(key_data_t);
            rows_in_keytable = _rows_in_keytable;
        }


// Поиск совпадения кода нажатой кнопки со значениями в таблице
uint8_t RemotePult::findPositionInTable(uint32_t _buttonCode) {
    uint8_t i;

    // Проход по всей таблице в поиске совпадающего кода
    for (i = 0; i < rows_in_keytable; i++) {
        if (_buttonCode == key_table[i].code) {
            break;
        }
    }

    // Если код обнаружен в таблице
    if (i < rows_in_keytable) return i;
    else return NOT_FOUND;
}

// Базовая функция работы пульта
// приём команд с пульта и их интерпретация
void RemotePult::process() {
    // Если таблица соответствия кодов не определена
    if (key_table == 0) return;
    
    // Если данные пришли
    //if (ir->decode()) {
    decode_results results;
    if (ir->decode(&results)) {
        newButtonCode = results.value;
        //newButtonCode = ir->decodedIRData.decodedRawData;
            
        // Сохранение времени последнего прихода данных
        lastButtonTime = millis();

        if (state == KEY_NO) {
            buttonPositionInTable = findPositionInTable(newButtonCode);
            if (buttonPositionInTable == NOT_FOUND) {

            } else {
                state = KEY_PRESSED;
                //debug(state);
                onButtonPressed(buttonPositionInTable);
                lastButtonCode = newButtonCode;
            }
        } else if (state == KEY_PRESSED) {
            if ((newButtonCode == 0x0)) {
                state = KEY_AUTOREPEAT;
                //debug(state);                   
                onButtonAutorepeat(buttonPositionInTable);
            }
        } else if (state == KEY_AUTOREPEAT) {
            //debug(state);
            //debug(buttonPositionInTable);
            onButtonAutorepeat(buttonPositionInTable);
        }
        ir->resume(); // принимаем следующую команду
    
    // Если данные не приходили
    } else {
        if ((state == KEY_PRESSED) || (state == KEY_AUTOREPEAT)) {
            timeEllapsed = millis() - lastButtonTime;
            if (timeEllapsed > AUTOREPEAT_PERIOD_MS) {
                state = KEY_RELEASED;
                //debug(state);
                //debug(timeEllapsed);
                onButtonReleased(buttonPositionInTable);
                state = KEY_NO;
                //debug(state);
            }
        }
    }
}

void RemotePult::onButtonPressed(uint8_t button) {
    pin_toggle(key_table[button].pin, key_table[button].var_val);
}

void RemotePult::onButtonReleased(uint8_t button) {
    pin_off(key_table[button].pin);
}

void RemotePult::onButtonAutorepeat(uint8_t button) {
    int16_t *var;
    uint8_t pin;
    // Если определена индексная ссылка на переменную
    if (key_table[button].var_ref) {
        // Получение ссылки на переменную
        var = &key_table[key_table[button].var_ref].var_val;
        pin = key_table[key_table[button].var_ref].pin;
        // Если ссылка не определена...
    } else {
        // ...то используется собственная переменная
        var = &key_table[button].var_val;
        pin = key_table[button].pin;
    }

    *var += key_table[button].var_step;
    pin_pwm(pin, (uint16_t) *var);
}

// Интерпретация нажатия кнопки (разные действия в зависимости типа кнопки заданного в табице)
void RemotePult::interpretateKeyActionByTableSettings(keyStates state, uint8_t button) {
    if (key_table[button].mode == MODE_TOGGLE) {
        if (state == KEY_PRESSED) {
            pin_toggle(key_table[button].pin, key_table[button].var_val);
        }
    }

    if (key_table[button].mode == MODE_HOLD) {
        if (state == KEY_AUTOREPEAT) {           
            int16_t *var;
            uint8_t pin;
            // Если определена индексная ссылка на переменную
            if (key_table[button].var_ref) {
                // Получение ссылки на переменную
                var = &key_table[key_table[button].var_ref].var_val;
                pin = key_table[key_table[button].var_ref].pin;
            // Если ссылка не определена...
            } else {
                // ...то используется собственная переменная
                var = &key_table[button].var_val;
                pin = key_table[button].pin;
            }

            *var += key_table[button].var_step;
            pin_pwm(pin, (uint16_t) *var);

        } else if (state == KEY_RELEASED) {
            pin_off(key_table[button].pin);
        }
    }
}


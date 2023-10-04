#include "input.hpp"
#include <Windows.h>

// See https://learn.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input and https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
bool key_is_down(const int virtual_key_code){

    return GetKeyState(virtual_key_code) & 0x8000; // Bit comparison on the key state that checks if the key is down.

}




bool left_arrow_key_is_down(){ return key_is_down(VK_LEFT); }

bool right_arrow_key_is_down(){ return key_is_down(VK_RIGHT); }

bool up_arrow_key_is_down(){ return key_is_down(VK_UP); }

bool down_arrow_key_is_down(){ return key_is_down(VK_DOWN); }

bool escape_key_is_down(){ return key_is_down(VK_ESCAPE); }

bool r_key_is_down(){ return key_is_down(0x52); }

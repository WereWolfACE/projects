#ifndef _KEYCODES_H_
#define _KEYCODES_H_

#if defined(ENGINE_TARGET_WIN32)

#define VK_0 '0'
#define VK_1 '1'
#define VK_2 '2'
#define VK_3 '3'
#define VK_4 '4'
#define VK_5 '5'
#define VK_6 '6'
#define VK_7 '7'
#define VK_8 '8'
#define VK_9 '9'
#define VK_A 'A'
#define VK_B 'B'
#define VK_C 'C'
#define VK_D 'D'
#define VK_E 'E'
#define VK_F 'F'
#define VK_G 'G'
#define VK_H 'H'
#define VK_I 'I'
#define VK_J 'J'
#define VK_K 'K'
#define VK_L 'L'
#define VK_M 'M'
#define VK_N 'N'
#define VK_O 'O'
#define VK_P 'P'
#define VK_Q 'Q'
#define VK_R 'R'
#define VK_S 'S'
#define VK_T 'T'
#define VK_U 'U'
#define VK_V 'V'
#define VK_W 'W'
#define VK_X 'X'
#define VK_Y 'Y'
#define VK_Z 'Z'

// Небуквенные символы. Делаем псевдонимы чтобы не выглядело очень страшно
// клавиша '[{'
#define VK_SQUARE_BRACE_LEFT 0xDB
#define VK_OEM_4 0xDB
// клавиша ']}'
#define VK_SQUARE_BRACE_RIGHT 0xDD
#define VK_OEM_6 0xDD

#elif defined(ENGINE_TARGET_MACOS)

#define VK_BACK 51
#define VK_TAB 48
#define VK_RETURN 36
#define VK_RETURN_NUMPAD 76 // на доп. клавиатуре
#define VK_PAUSE 113
#define VK_ESCAPE 53
#define VK_SPACE 49
#define VK_PRIOR 116 //PgUp
#define VK_NEXT 121 // PgDn
#define VK_END 119
#define VK_HOME 115
#define VK_LEFT 123
#define VK_UP 126
#define VK_RIGHT 124
#define VK_DOWN 125
#define VK_SELECT 110
#define VK_SNAPSHOT 105
#define VK_INSERT 114
#define VK_DELETE 117
#define VK_SHIFT 56
#define VK_MENU 58
#define VK_CONTROL 59

#define VK_0 29
#define VK_1 18
#define VK_2 19
#define VK_3 20
#define VK_4 21
#define VK_5 23
#define VK_6 22
#define VK_7 26
#define VK_8 28
#define VK_9 25
#define VK_A 0
#define VK_B 11
#define VK_C 8
#define VK_D 2
#define VK_E 14
#define VK_F 3
#define VK_G 5
#define VK_H 4
#define VK_I 34
#define VK_J 38
#define VK_K 40
#define VK_L 37
#define VK_M 46
#define VK_N 45
#define VK_O 31
#define VK_P 35
#define VK_Q 12
#define VK_R 15
#define VK_S 1
#define VK_T 17
#define VK_U 32
#define VK_V 9
#define VK_W 13
#define VK_X 7
#define VK_Y 16
#define VK_Z 6

#define VK_NUMPAD0 82
#define VK_NUMPAD1 83
#define VK_NUMPAD2 84
#define VK_NUMPAD3 85
#define VK_NUMPAD4 86
#define VK_NUMPAD5 87
#define VK_NUMPAD6 88
#define VK_NUMPAD7 89
#define VK_NUMPAD8 91
#define VK_NUMPAD9 92

#define VK_MULTIPLY 67
#define VK_ADD 69
#define VK_SUBTRACT 78
#define VK_DECIMAL 65
#define VK_DIVIDE 75

#define VK_F1 122
#define VK_F2 120
#define VK_F3 99
#define VK_F4 118
#define VK_F5 96
#define VK_F6 97
#define VK_F7 98
#define VK_F8 100
#define VK_F9 101
#define VK_F10 109
#define VK_F11 103
#define VK_F12 111

#define VK_NUMLOCK 71
#define VK_SCROLL 107
#define VK_OEM_1 41 // ;:
#define VK_OEM_PLUS 24
#define VK_OEM_COMMA 43
#define VK_OEM_MINUS 27
#define VK_OEM_PERIOD 47
#define VK_OEM_2 44 // /?
#define VK_OEM_3 50 // ~
#define VK_OEM_4 33 // [{
#define VK_SQUARE_BRACE_LEFT 33
#define VK_OEM_5 42 // \|
#define VK_OEM_6 30 // ]}
#define VK_SQUARE_BRACE_RIGHT 30
#define VK_OEM_7 39 // '"

#elif defined(ENGINE_TARGET_IPHONE) || defined(ENGINE_TARGET_ANDROID)

#define VK_RETURN 0x09
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define VK_BACK 0x08

#endif // ENGINE_TARGET_

#endif // _KEYCODES_H_

#ifndef _CHARCODES_H_
#define _CHARCODES_H_

// Коды "особых" символов, для читаемости

#define CHAR_CODE_RETURN 13
#define CHAR_CODE_ESCAPE 27
#define CHAR_CODE_SPACE 32

#if defined(ENGINE_TARGET_MACOS)
#define CHAR_CODE_BACK 127
#else
#define CHAR_CODE_BACK 8
#endif // ENGINE_TARGET_MACOS

#endif // _CHARCODES_H_

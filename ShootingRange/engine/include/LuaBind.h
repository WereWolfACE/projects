#ifndef _UTILITIES_LUABIND_H_
#define _UTILITIES_LUABIND_H_

// «‡„ÓÎÓ‚ÓÍ ‰Îˇ ‚ÍÎ˛˜ÂÌËˇ luabind.
// ÕÛÊÂÌ, ÔÓÚÓÏÛ ˜ÚÓ Ú‡Ï ÂÒÚ¸ ËÌÚÂÂÒÌ‡ˇ ÍÓÌÒÚÛÍˆËˇ:
// ‚ assert ËÒÔÓÎ¸ÁÛÂÚÒˇ ÔÂÂÏÂÌÌ‡ˇ, ÍÓÚÓ‡ˇ ‚ ÂÊËÏÂ NDEBUG ÌÂ ÓÔÂ‰ÂÎÂÌ‡.
// —Ú‡Ì‰‡ÚÌ˚È assert ÔÓÒÚÓ ‚Ò∏ ‚˚‡ÊÂÌËÂ ‚˚ÂÁ‡ÂÚ,
// ‡ Ì‡¯ ‡·ÓÚ‡ÂÚ Ë ‚ ÂÊËÏÂ NDEBUG ÚÓÊÂ,
// ÔÓ˝ÚÓÏÛ luabind ·ÂÁ ˝ÚÓ„Ó Á‡„ÓÎÓ‚Í‡ ÌÂ ÍÓÏÔËÎËÛÂÚÒˇ.

#ifdef NDEBUG
#define m_cloned false
#endif //NDEBUG

// на MacOS отключаем системный макрос check, иначе будет конфликт с luabind
#undef check

#undef nil
#define nil _nil

#include <luabind/luabind.hpp>

#undef nil
#define nil __null

//#undef check
//#define check __null

#ifdef NDEBUG
#undef m_cloned
#endif


#endif

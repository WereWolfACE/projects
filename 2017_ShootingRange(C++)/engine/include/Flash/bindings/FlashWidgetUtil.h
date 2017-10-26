#pragma once

const float DESIGN_WIDTH = 1024.0f;
const float DESIGN_HEIGHT = 768.0f;
const float FPS = 35.0f;
const float INVFPS = 1.0f / FPS;

inline int selector(const char* scrutinee, const char* v0, const char* v1){
	if ( scrutinee == 0 )
		return 0;
	if ( !::strcmp(scrutinee, v0) )
		return 0;
	if ( !::strcmp(scrutinee, v1) )
		return 1;
	Assert(false);
    return 0;
}

template<class T>
inline T maybeParse(T def, const char* a){
    if ( a == 0 )
        return def;
	return utils::lexical_cast<T>(a);
}

template<>
inline std::string maybeParse(std::string def, const char* a){
    if ( a == 0 )
        return def;
	return std::string(a);
}

#ifndef _UTILS_COLOR_H_
#define _UTILS_COLOR_H_

#pragma once

#ifndef RAPIDXML_STATIC_POOL_SIZE
#define RAPIDXML_STATIC_POOL_SIZE 10 * 1024
#endif
#include "../rapidxml/rapidxml.hpp"

#include <cstdint>

#define MAKECOLOR4(a, r, g, b) Color(r, g, b, a)
#define RGBA_SWAP_RB(x) (((x) & 0xff00ff00) | (((x) & 0xff0000) >> 16) | (((x) & 0x0000ff) << 16))

struct Color {
	static const Color WHITE;	
	static const Color BLACK;	
	static const Color RED;	
	static const Color GREEN;	
	static const Color BLUE;	
	static const Color BLACK_TRANSPARENT;	
	static const Color WHITE_TRANSPARENT;
	
	union {
		uint8_t pColor[4];
		uint32_t rgba;
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t alpha;
		};
	};
	
	Color();	
	Color(uint8_t red, uint8_t green, uint8_t blue);	
	Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);	
	explicit Color(rapidxml::xml_node<>* elem);
	
	//
	// Читает цвет из строки.
	// Цвет задаётся в двух возможных форматах:
	// 1) "#RRGGBBAA", например #FFFFFFFF, #A10000;
	// 2) "RRR;GGG;BBB;AAA", например "128;255;60;50".
	//
	explicit Color(const char* c);
	explicit Color(const std::string& c);
	
	explicit Color(uint32_t rgba);
	explicit Color(uint8_t alpha);

	Color& operator*=(const Color& c);
	
	bool operator==(const Color& c) const;
	
	bool operator!=(const Color& c) const { return !operator == (c); }
};

Color operator*(const Color& c1, const Color& c2);
Color operator*(const Color& c, float k);
Color operator*(float k, const Color& c);
Color operator+(const Color& c1, const Color& c2);

#endif //_UTILS_COLOR_H_

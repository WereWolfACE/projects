#ifndef __UTILS_XML_H__
#define __UTILS_XML_H__

#pragma once

#include "Utils/Color.h"
#include "Utils/IPoint.h"
#include "Utils/FPoint.h"
#include "Utils/IRect.h"
#include "Utils/FRect.h"
#include "Utils/Vector3.h"
#include "Utils/utils.hpp"

#include <pugixml.hpp>

#ifndef RAPIDXML_STATIC_POOL_SIZE
#define RAPIDXML_STATIC_POOL_SIZE 10 * 1024
#endif
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../rapidxml/rapidxml_print.hpp"

#define QueryNvpAttribute(elem, attr) Xml::QueryAttribute(elem, #attr, attr)

//
// Функции для более удобной работы с TinyXml.
// (установка и получение XML-атрибутов)
//
namespace Xml {

// pugixml

// Есть ли у xe атрибут name.
bool HasAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа float; атрибут с таким именем обязан существовать.
float GetFloatAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа double; атрибут с таким именем обязан существовать.
double GetDoubleAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа bool; атрибут с таким именем обязан существовать.
bool GetBoolAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа int; атрибут с таким именем обязан существовать.
int GetIntAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа int64; атрибут с таким именем обязан существовать.
int64_t GetInt64Attribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа color; атрибут с таким именем обязан существовать.
Color GetColorAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа std::string; атрибут с таким именем обязан существовать.
std::string GetStringAttribute(pugi::xml_node xe, const std::string& name);

// Получить из элемента xe атрибут типа float или вернуть значение по умолчанию
float GetFloatAttributeOrDef(pugi::xml_node xe, const std::string& name, float defaultValue);

// Получить из элемента xe атрибут типа double или вернуть значение по умолчанию
double GetDoubleAttributeOrDef(pugi::xml_node xe, const std::string& name, double defaultValue);

// Получить из элемента xe атрибут типа bool или вернуть значение по умолчанию
bool GetBoolAttributeOrDef(pugi::xml_node xe, const std::string& name, bool defaultValue);

// Получить из элемента xe атрибут типа int или вернуть значение по умолчанию
int GetIntAttributeOrDef(pugi::xml_node xe, const std::string& name, int defaultValue);

// Получить из элемента xe атрибут типа int64 или вернуть значение по умолчанию
int64_t GetInt64AttributeOrDef(pugi::xml_node xe, const std::string& name, int64_t defaultValue);

// Получить из элемента xe атрибут типа color или вернуть значение по умолчанию
Color GetColorAttributeOrDef(pugi::xml_node xe, const std::string& name, Color defaultValue);

// Получить из элемента xe атрибут типа std::string или вернуть значение по умолчанию
std::string GetStringAttributeOrDef(pugi::xml_node xe, const std::string& name, const std::string& defaultValue);

// Установить в элементе xe атрибут типа bool
void SetBoolAttribute(pugi::xml_node xe, const std::string& name, bool value);

// Установить в элементе xe атрибут типа int
void SetIntAttribute(pugi::xml_node xe, const std::string& name, int value);

// Установить в элементе xe атрибут типа int64
void SetInt64Attribute(pugi::xml_node xe, const std::string& name, int64_t value);

// Установить в элементе xe атрибут типа float
void SetFloatAttribute(pugi::xml_node xe, const std::string& name, float value);

// Установить в элементе xe атрибут типа double
void SetDoubleAttribute(pugi::xml_node xe, const std::string& name, double value);

// Установить в элементе xe атрибут типа std::string
void SetStringAttribute(pugi::xml_node xe, const std::string& name, const std::string& value);

// Установить в элементе xe атрибут типа Color
void SetColorAttribute(pugi::xml_node xe, const std::string& name, const Color& value);

// Создать объект IPoint из xml атрибутов x, y
IPoint IPointFromXml(pugi::xml_node xe);

// Создать объект FPoint из xml атрибутов x, y
FPoint FPointFromXml(pugi::xml_node xe);

// Создать объект IRect из xml атрибутов x, y, width, height
IRect IRectFromXml(pugi::xml_node xe);

// Создать объект FRect из xml атрибутов xStart, yStart, xEnd, yEnd
FRect FRectFromXml(pugi::xml_node xe);

// Создать объект math::Vector3 из xml атрибутов x, y, z
math::Vector3 Vector3FromXml(pugi::xml_node xe);

// Создаёт новый узел
pugi::xml_node NewNode(pugi::xml_node xe, const char* name, const char* value = NULL);

template <class T>
inline bool QueryAttribute(pugi::xml_node elem, const std::string& attrName, T& attrValue)
{
	auto attr = elem.attribute(attrName.c_str());
	if (attr) {
		attrValue = utils::lexical_cast<T>(attr.value());
		return true;
	}

	return false;
}

template <>
inline bool QueryAttribute(pugi::xml_node elem, const std::string& attrName, std::string& attrValue)
{
	auto attr = elem.attribute(attrName.c_str());
	if (attr) {
		attrValue = attr.value();
		return true;
	}
	return false;
}

// rapidxml

// Есть ли у xe атрибут name.
bool HasAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа float; атрибут с таким именем обязан существовать.
float GetFloatAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа double; атрибут с таким именем обязан существовать.
double GetDoubleAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа bool; атрибут с таким именем обязан существовать.
bool GetBoolAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа int; атрибут с таким именем обязан существовать.
int GetIntAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа int64; атрибут с таким именем обязан существовать.
int64_t GetInt64Attribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа color; атрибут с таким именем обязан существовать.
Color GetColorAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа std::string; атрибут с таким именем обязан существовать.
std::string GetStringAttribute(const rapidxml::xml_node<>* xe, const std::string& name);

// Получить из элемента xe атрибут типа float или вернуть значение по умолчанию
float GetFloatAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, float defaultValue);

// Получить из элемента xe атрибут типа double или вернуть значение по умолчанию
double GetDoubleAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, double defaultValue);

// Получить из элемента xe атрибут типа bool или вернуть значение по умолчанию
bool GetBoolAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, bool defaultValue);

// Получить из элемента xe атрибут типа int или вернуть значение по умолчанию
int GetIntAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, int defaultValue);

// Получить из элемента xe атрибут типа int64 или вернуть значение по умолчанию
int64_t GetInt64AttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, int64_t defaultValue);

// Получить из элемента xe атрибут типа color или вернуть значение по умолчанию
Color GetColorAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, Color defaultValue);

// Получить из элемента xe атрибут типа std::string или вернуть значение по умолчанию
std::string GetStringAttributeOrDef(const rapidxml::xml_node<>* xe, const std::string& name, const std::string& defaultValue);

// Установить в элементе xe атрибут типа std::string
void SetStringAttribute(rapidxml::xml_node<>* xe, const std::string& name, const std::string& value);

// Установить в элементе xe атрибут типа bool
void SetBoolAttribute(rapidxml::xml_node<>* xe, const std::string& name, bool value);

// Установить в элементе xe атрибут типа int
void SetIntAttribute(rapidxml::xml_node<>* xe, const std::string& name, int value);

// Установить в элементе xe атрибут типа int64
void SetInt64Attribute(rapidxml::xml_node<>* xe, const std::string& name, int64_t value);

// Установить в элементе xe атрибут типа float
void SetFloatAttribute(rapidxml::xml_node<>* xe, const std::string& name, float value);

// Установить в элементе xe атрибут типа double
void SetDoubleAttribute(rapidxml::xml_node<>* xe, const std::string& name, double value);

// Установить в элементе xe атрибут типа Color
void SetColorAttribute(rapidxml::xml_node<>* xe, const std::string& name, const Color& value);

// Создать объект IPoint из xml атрибутов x, y
IPoint IPointFromXml(const rapidxml::xml_node<>* xe);

// Создать объект FPoint из xml атрибутов x, y
FPoint FPointFromXml(const rapidxml::xml_node<>* xe);

// Создать объект IRect из xml атрибутов x, y, width, height
IRect IRectFromXml(const rapidxml::xml_node<>* xe);

// Создать объект FRect из xml атрибутов xStart, yStart, xEnd, yEnd
FRect FRectFromXml(const rapidxml::xml_node<>* xe);

// Создать объект math::Vector3 из xml атрибутов x, y, z
math::Vector3 Vector3FromXml(const rapidxml::xml_node<>* xe);

// Создаёт новый узел
rapidxml::xml_node<>* NewNode(rapidxml::xml_node<>* xe, const char* name, const char* value = NULL);

template <class T>
inline bool QueryAttribute(const rapidxml::xml_node<>* elem, const std::string& attrName, T& attrValue)
{
	auto attr = elem ? elem->first_attribute(attrName.c_str()) : nullptr;
	if (attr) {
		attrValue = utils::lexical_cast<T>(attr->value());
		return true;
	}

	return false;
}

template <>
inline bool QueryAttribute(const rapidxml::xml_node<>* elem, const std::string& attrName, std::string& attrValue)
{
	auto attr = elem ? elem->first_attribute(attrName.c_str()) : nullptr;
	if (attr) {
		attrValue = attr->value();
		return true;
	}
	return false;
}

// Долгоживущий rapidxml::xml_document
class RapidXmlDocument : public RefCounter {
public:
	enum ParseMode { ParseDefault, ParseFull };

	RapidXmlDocument(const std::string &name, ParseMode mode = ParseDefault)
		: filename(name)
		, mode(mode)
	{
		Reload();
	}

	void Reload();

	rapidxml::xml_node<> *first_node(const char* name = 0) const {
		return doc.first_node(name);
	}

	rapidxml::xml_document<>& GetDocument() {
		return doc;
	}

	const char* GetData() const {
		return file ? file->data() : nullptr;
	}

	size_t GetSize() const {
		return file ? file->size() : 0;
	}

private:
	std::string filename;
	std::unique_ptr<rapidxml::file<>> file;
	rapidxml::xml_document<> doc;
	ParseMode mode;
};

typedef boost::intrusive_ptr<RapidXmlDocument> RapidXmlDocumentPtr;

} // namespace Xml

#endif // __UTILS_XML_H__

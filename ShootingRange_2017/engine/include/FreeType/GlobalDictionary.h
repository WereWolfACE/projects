#pragma once

#include <map>
#include <string>

#include "IDictionary.h"
#include "ITextElement.h"

namespace freetype {
class GlobalDictionary : public IDictionary {
public:
	void setConstant(const std::string& id, const std::string& value);
	void setVariable(const std::string& id, const std::string& value);
	ITextElement* get(const std::string& id) const;

private:
	std::map<std::string, std::string*> variableRefs;
	std::map<std::string, ITextElementPtr> variables;

};

/**
* ¬озвращает глобальный словарь подстановочных символов, используемый текстами
*/
GlobalDictionary* getGlobalDictionary();
};

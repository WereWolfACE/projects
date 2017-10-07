#pragma once

#include <string>
#include <boost/intrusive_ptr.hpp>

#include <RefCounter.h>

class ITextElement;

class IDictionary : public RefCounter {
public:
	virtual ITextElement* get(const std::string& id) const = 0;
};

typedef boost::intrusive_ptr<IDictionary> IDictionaryPtr;

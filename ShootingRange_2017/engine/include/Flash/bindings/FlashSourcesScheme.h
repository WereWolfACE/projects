#pragma once

#include "Flash/bindings/FlashSource.h"
#include "RefCounter.h"

class FlashSourcesScheme : public RefCounter {
public:
	static FlashSourcesScheme* parse(rapidxml::xml_node<>* xml);

	int countSource() const;
	FlashSource* getSource(int index) const;

protected:
	void doParse(rapidxml::xml_node<>* xml);

private:
	std::vector<FlashSourcePtr> source;
};

typedef boost::intrusive_ptr<FlashSourcesScheme> FlashSourcesSchemePtr;

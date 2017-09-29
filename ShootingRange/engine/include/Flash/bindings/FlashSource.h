#pragma once

#include "RefCounter.h"

class FlashSource : public RefCounter {
public:
	static FlashSource* parse(rapidxml::xml_node<>* xml);
	const std::string& getFile() const;
	const std::string& getPath() const;

protected:
	void doParse(rapidxml::xml_node<>* xml);

private:
	std::string file;
	std::string path;
};

typedef boost::intrusive_ptr<FlashSource> FlashSourcePtr;

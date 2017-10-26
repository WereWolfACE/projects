#pragma once

class ICleanable;

void addCleanableResource(ICleanable* cleanable);
void removeCleanableResource(ICleanable* cleanable);
void cleanupResources();

class ICleanable{
public:
	ICleanable(){
		addCleanableResource(this);
	}

	virtual ~ICleanable(){
		removeCleanableResource(this);
	}

	virtual void clean() = 0;
};


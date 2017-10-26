#ifndef _MESSAGEMANAGER_H_
#define _MESSAGEMANAGER_H_

#include "Utils/Color.h"
#include "Utils/FPoint.h"
#include "Utils/IPoint.h"
#include "Utils/Vector3.h"

#include "ThreadSupport.h"
#include "LuaBind.h"

#include <boost/variant.hpp>

namespace GUI {
	class Widget;
}

typedef boost::variant< 
		bool,
		int, 
		float, 
		std::string,
		Color,
		IPoint,
		FPoint,
		math::Vector3,
		GUI::Widget*,
		boost::function<void()>,
		luabind::object
	> lvariant;

class VariableSet
{
public:
	bool isEmpty() const;
	
	bool findName(const std::string& name);

	bool getBool(const std::string& varName);

	int getInt(const std::string& varName);

	float getFloat(const std::string& varName);

	std::string getString(const std::string& varName);

	Color getColor(const std::string& varName);

	IPoint getPoint(const std::string& varName);

	FPoint getFPoint(const std::string& varName);
	
	math::Vector3 getVector3(const std::string& varName);

	GUI::Widget* getWidget(const std::string& varName);

	boost::function<void()> getFunction(const std::string& varName);

	luabind::object getLuaObject(const std::string& varName);

	void setBool(const std::string& varName, bool value);

	void setInt(const std::string& varName, int value);

	void setFloat(const std::string& varName, float value);

	void setString(const std::string& varName, const std::string& value);

	void setColor(const std::string& varName, const Color& value);

	void setPoint(const std::string& varName, const IPoint& value);

	void setFPoint(const std::string& varName, const FPoint& value);
	
	void setVector3(const std::string& varName, const math::Vector3& value);

	void setWidget(const std::string& varName, GUI::Widget* value);

	void setFunction(const std::string& varName, boost::function<void()> value);

	void setLuaObject(const std::string& varName, luabind::object value);

	void reset();

	void eraseName(const std::string &name);
	
	void load(rapidxml::xml_node<> *element);

	std::string GetAsString(const std::string& varName) const;

private:
	typedef std::map<std::string, lvariant> VarSet;

	VarSet _varSet;
	
	std::string GetAsString(VarSet::const_iterator& it) const;
	
	friend std::ostream& operator<<(std::ostream &os, const VariableSet& v);
};

namespace Core
{
	class MessageManagerImpl;
}

class Message
{
	friend class Core::MessageManagerImpl;

	std::string _publisher;
	std::string _targetLayer; // какому слою передавать, если = "" передавать activelayer
	std::string _data;
	int _param;  

public:
	mutable VariableSet variables;

	Message();

	Message(const std::string& publisher);

    Message(const std::string& publisher, const std::string& data);

	Message (const std::string& publisher, int param);

	Message (const std::string& publisher, const std::string& data, int param);

	Message (const int param);

	~Message();
	
	const std::string& getPublisher() const;

	const std::string& getData() const;

	int getIntegerParam() const;

	VariableSet& getVariables() const;

	bool is(const std::string& publisher, const std::string& data) const;

	bool is(const std::string& publisher) const;

	void SetTargetLayer(const std::string& targetLayer);

	const std::string& GetTargetLayer() const;
	
	Message& To(const std::string& targetLayer);
	
	friend std::ostream& operator<<(std::ostream &os, const Message& msg);
};

namespace Core
{

class MessageManagerImpl 
{
public:
	bool getMessage();

    Message popMessage();

	void putMessage(const Message& message);

	void UpdatePostMessageQueue();

	void Clear();

	void ClearAll();

private:
	typedef std::deque<Message> Messages;

	MUTEX_TYPE _mutex;
	
	Messages _messageQueue;
	Messages _postMessageQueue;
};

extern MessageManagerImpl messageManager;

}

#endif
#ifndef __FACTORY_H__
#define __FACTORY_H__

#pragma once

#include "Core/MessageManager.h"
#include "Core/ControllerManager.h"
#include "GUI/Widget.h"
#include "Utils/Xml.h"

template<typename BaseClassType, typename ClassType>
BaseClassType *CreateObjectByVarList(VariableSet& varSet)
{
	return new ClassType(varSet);
}

template<typename BaseClassType, typename UniqueIdType>
class VarlistObjectFactory
{
protected:
	typedef BaseClassType *(*CreateObjectFunc)(VariableSet&);

public:
	typedef typename std::map<UniqueIdType, CreateObjectFunc>::const_iterator ConstIterator;
	typedef typename std::map<UniqueIdType, CreateObjectFunc>::iterator Iterator;


	template<typename ClassType>
		bool Register(UniqueIdType unique_id)
	{
		if (m_object_creator.find(unique_id) != m_object_creator.end())
			return false;

		m_object_creator[unique_id] = &CreateObjectByVarList<BaseClassType, ClassType>;

		return true;
	}

	bool Unregister(UniqueIdType unique_id)
	{
		return (m_object_creator.erase(unique_id) == 1);
	}

	BaseClassType *Create(UniqueIdType unique_id, VariableSet& varSet)
	{
		Iterator iter = m_object_creator.find(unique_id);

		if (iter == m_object_creator.end())
			return NULL;

		return ((*iter).second)(varSet);
	}

	ConstIterator GetBegin() const
	{
		return m_object_creator.begin();
	}

	Iterator GetBegin()
	{
		return m_object_creator.begin();
	}

	ConstIterator GetEnd() const
	{
		return m_object_creator.end();
	}

	Iterator GetEnd()
	{
		return m_object_creator.end();
	}

protected:
	std::map<UniqueIdType, CreateObjectFunc> m_object_creator;
};

template<typename WidgetType>
GUI::Widget *CreateWidgetFromXml(rapidxml::xml_node<>* elem)
{
	std::string widgetName;
	if (!Xml::QueryAttribute(elem, "name", widgetName))
	{
		//Assert(false); // пропущено имя
		//return NULL;
	}
	return new WidgetType(widgetName, elem);
}

class XmlWidgetFactory
{
protected:
	typedef GUI::Widget *(*CreateObjectFunc)(rapidxml::xml_node<>* elem);

public:
	typedef std::map<std::string, CreateObjectFunc>::const_iterator ConstIterator;
	typedef std::map<std::string, CreateObjectFunc>::iterator Iterator;


	template<typename WidgetType>
	bool Register(const std::string& unique_id)
	{
		if (m_object_creator.find(unique_id) != m_object_creator.end())
			return false;

		m_object_creator[unique_id] = &CreateWidgetFromXml<WidgetType>;

		return true;
	}

	bool Unregister(const std::string& unique_id)
	{
		return (m_object_creator.erase(unique_id) == 1);
	}

	GUI::Widget *Create(rapidxml::xml_node<>* elem)
	{
		std::string widgetClass;
		widgetClass = std::string(elem->name());
		Iterator iter = m_object_creator.find(widgetClass);
		if (iter == m_object_creator.end()) {
			Assert(false); // вываливаемся здесь, чтобы видеть какого виджета не хватает
			return NULL;
		}

		return ((*iter).second)(elem);
	}
	
	// рекурсивный вариант предыдущего метода
	GUI::Widget *CreateTree(rapidxml::xml_node<>* elem);

	ConstIterator GetBegin() const
	{
		return m_object_creator.begin();
	}

	Iterator GetBegin()
	{
		return m_object_creator.begin();
	}

	ConstIterator GetEnd() const
	{
		return m_object_creator.end();
	}

	Iterator GetEnd()
	{
		return m_object_creator.end();
	}

protected:
	std::map<std::string, CreateObjectFunc> m_object_creator;
};

namespace Factory
{
	typedef VarlistObjectFactory<IController, std::string> TControllerFactory;
	extern TControllerFactory controllerFactory;
	extern XmlWidgetFactory widgetFactoryXml;
	void RegisterTypes();
}

#define REGISTER_CONTROLLER(name) Factory::controllerFactory.Register<name>(#name)
#define REGISTER_WIDGET_XML(className, regName) Factory::widgetFactoryXml.Register<className>(regName)
#define REGISTER_WIDGET_XML_AUTO(className) Factory::widgetFactoryXml.Register<className>(#className)
#define UNREGISTER_WIDGET_XML(regName) Factory::widgetFactoryXml.Unregister(regName)

#endif // __FACTORY_H__

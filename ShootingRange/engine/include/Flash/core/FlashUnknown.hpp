#pragma once

#include "GCRef.h"

/*
	Здесь реализована очень примитивная возможность кастовать интерфейсы между собой, без использования dynamic_cast,
	по сути дела это не что иное как очень примитивная реализация итерфейса подобного IUnknown(https://msdn.microsoft.com/en-us/library/windows/desktop/ms680509%28v=vs.85%29.aspx),
	однако вместо универсального QueryInterfeace У интрефейса IFlashUnknown, есть россыпь методов по кастованю из всего во все,
	например вы клонировали объект

	IPrototype* l_objClone = l_obj->Clone();
	и спрашиватес ятчо делать с этим IPrototype*, но вспонинаем что все интрефейсы во флеше наследники от IFlashUnknown,
	мы можем заполучить нужный нам интрерфейс без особых хлопот:

	IFlashSpirte* l_objCloneSprite =  l_objClone->Sprite();
	да разумеется это можно было бы записать и так

	IFlashSpirte* l_objCloneSprite =  dynamic_cast<IFlashSpirte*>l_objClone;

	и это даже сработало бы, но dynamic_cast сам по себе штука во первых не быстрая, потому как в основе себя он содержит strcmp, а во вторых он зависит от rtti
	который можно и не генерировать (например ключом gcc компилятора -fno-rtti) для уменьшения объема генерируемого модуля


	Чтобы сказать что класс имплементирует определенный набор интрефейсов в его декларацию следует внести такую табилцу:

	class CFlashClass: public FlashDisplayObject<IMovieClip>, public IPrototype
	{
		BEGIN_INTERFACE_MAP()
			INTERFACE_ENTRY(IPrototype)
			INTERFACE_ENTRY(IFlashMovieClip)
			INTERFACE_ENTRY(IFlashSprite)
			INTERFACE_ENTRY(IFlashDisplayObject)
		END_INTERFACE_MAP()
		....
	}

	Для того чтобы создать объект класса CFlashClass, потребуется следующая контсрукция:

	IFlashDisplayObject* l_obj = new CFlashUnknown<CFlashClass>

	За более подробными объяснениями следует обращаться к коментарию для класса GCRef

	Генерация таблицы доступных интрефейсов может показаться дикостью из каменного века, и можно скзаать что раз есть пачка методов то и реализовывай те её в самих потомках,
	но тут возникает проблема, что в потомке придется реализовывать все методы пачки,
	и изменять все классы если добавиться новый интрефейс который нужно будет кастовать, считаю что подход с макросами тут более универсален и прост для расширения,
	к тому же все подобные решения сперты из ATL(https://ru.wikipedia.org/wiki/Active_Template_Library)
*/
enum FlashInterfaceIDD
{
	IDD_None = 0,
	IDD_IFlashDisplayObject,
	IDD_IFlashSprite,
	IDD_IPrototype,
	IDD_IFlashRasterSprite,
	IDD_IFlashMovieClip,
	IDD_IFlashText,
	IDD_IFlashParticleEffect,
	IDD_IScrollingContainer,
	IDD_IFlashScrollableFrame
};

#define BEGIN_INTERFACE_MAP() public: \
static bool __HaveInterface(FlashInterfaceIDD _intfidd) \
{\
	static const FlashInterfaceIDD _entries[] = {

#define INTERFACE_ENTRY(i) IDD_##i,

#define END_INTERFACE_MAP() IDD_None};\
	int l_i = 0;\
\
	while(_entries[l_i] != 0)\
	{\
		if(_entries[l_i++] == _intfidd)\
		{\
			return true;\
		};\
	};\
\
	return false;\
};

//-----------------------------------------------------------------------------
template<class T>
class CFlashUnknown: public CGCRef<T>
{
	IPrototype* Prototype() override
	{
		if(this->__HaveInterface(IDD_IPrototype))
		{
			return (IPrototype*)(this);
		};

		return NULL;
	};

	IFlashDisplayObject* DisplayObject() override
	{
		if(this->__HaveInterface(IDD_IFlashDisplayObject))
		{
			return (IFlashDisplayObject*)(this);
		};

		return NULL;
	};

	IFlashText* Text() override
	{
		if(this->__HaveInterface(IDD_IFlashText))
		{
			return (IFlashText*)(this);
		};

		return NULL;
	};

	IFlashRasterSprite* RasterSprite() override
	{
		if(this->__HaveInterface(IDD_IFlashRasterSprite))
		{
			return (IFlashRasterSprite*)(this);
		};

		return NULL;
	};

	IFlashSprite* Sprite() override
	{
		if(this->__HaveInterface(IDD_IFlashSprite))
		{
			return (IFlashSprite*)(this);
		};

		return NULL;
	};

	IFlashMovieClip* MovieClip() override
	{
		if(this->__HaveInterface(IDD_IFlashMovieClip))
		{
			return (IFlashMovieClip*)(this);
		};

		return NULL;
	};

	IFlashParticleEffect* ParticleEffect() override
	{
		if(this->__HaveInterface(IDD_IFlashParticleEffect))
		{
			return (IFlashParticleEffect*)(this);
		};

		return NULL;
	}

	IScrollingContainer* ScrollingContainer() override
	{
		if(this->__HaveInterface(IDD_IScrollingContainer))
		{
			return (IScrollingContainer*)(this);
		};

		return NULL;
	}

	IFlashScrollableFrame* ScrollableFrame() override
	{
		if (this->__HaveInterface(IDD_IFlashScrollableFrame))
		{
			return (IFlashScrollableFrame*)(this);
		};

		return NULL;
	}
};

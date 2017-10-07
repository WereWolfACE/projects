#pragma once

#include <Utils/Color.h>

#include "Flash/core/IPlaybackOperation.h"
#include "Flash/core/IFlashMovieClip.h"
#include "Flash/events/EventManager.h"
#include "Flash/events/FlashEvents.h"
#include "Flash/bindings/ScrollingContainer.h"
#include "Flash/bindings/FlashResourceManager.h"
#include "../core/FlashMarkSweep.h"
#include "../core/GCRef.h"
#include "../core/Prototype.h"
#include "../core/FlashSprite.h"
#include "../core/IFlashText.h"

#ifdef check
#undef check
#endif

namespace fquery
{

	class FQueryEventsPlaybackOperation: public IPlaybackOperation {
	public:
		void onFrameConstructed(IFlashMovieClip* movieClip, int frame){
			PlaybackEvent event(PlaybackFrame, frame);
			EventManager::Get().dispatchTarget(movieClip, event);
		}

		bool onAnimationCompleted(IFlashMovieClip* movieClip){
			PlaybackEvent event(PlaybackEnd, movieClip->getCurrentFrame());
			EventManager::Get().dispatchTarget(movieClip, event);
			return !(event.isDefaultPrevented());
		}

		void onAnimationRestarted(IFlashMovieClip* movieClip){
			PlaybackEvent event(PlaybackRestarted, movieClip->getCurrentFrame());
			EventManager::Get().dispatchTarget(movieClip, event);
		}

		void onMovieDisposed(IFlashMovieClip* movieClip){
		}
	};

class LocalStorage: public IGCRef
{
public:
	LocalStorage()
	{
	};

	virtual ~LocalStorage()
	{
	};

	template<class T>
	T* get(const std::string& id)
	{
		return dynamic_cast<T*>(data.at(id));
	};

	template<class T>
	void set(const std::string& id, T* t){
		data.emplace(id, t);
	}

	bool has(const std::string& id){
		return data.count(id) > 0;
	}

	IGCRef* get_raw(const std::string& id){
		return data.at(id);
	}

	void set_raw(const std::string& id, IGCRef* value){
		data.emplace(id, value);
	}

	GC_BLACKEN_DECL()
	{
		for ( auto it = data.begin(); it != data.end(); ++it)
		{
			GC_SHOULD_MARK(it->second);
		}
	}
private:
	std::map<std::string, IGCRef*> data;
};

class LocalStorageDictionary: public IGCRef
{
public:
	LocalStorageDictionary()
	{
	};

	virtual ~LocalStorageDictionary()
	{
	};

	void removeStorage(IFlashDisplayObject* _target)
	{
		auto l_targetIt = storages.find(_target);

		if ( l_targetIt != storages.end() )
		{
			storages.erase(l_targetIt);
		};
	};

	LocalStorage* getStorage(IFlashDisplayObject* _target)
	{
		auto l_targetIt = storages.find(_target);
		CGCRef<LocalStorage>* l_storage = NULL;

		if (l_targetIt == storages.end() )
		{
			l_storage = new  CGCRef<LocalStorage>();
			GC_SHOULD_TRACK(l_storage);

			storages.emplace(_target, l_storage);
		}
		else
		{
			l_storage = l_targetIt->second;
		};

		return l_storage;
	};

	GC_BLACKEN_DECL()
	{
		for ( auto it = storages.begin(); it != storages.end(); ++it)
		{
			GC_SHOULD_MARK(it->second);
		};
	};

private:
	std::map<IFlashDisplayObject*, CGCRef<LocalStorage>*> storages;
};

	struct Vec2{
		float x, y;
		Vec2(){
			x = y = 0;
		}
		Vec2(float x, float y){
			this->x = x;
			this->y = y;
		}
	};

	static inline int gcd(int a, int b){
		if ( b == 0 )
			return a;
		return gcd(b, a % b);
	}

	static inline int lcd(int a, int b){
		return a * b / gcd(a, b);
	}

class FQuerySelector
{
	static const int InplaceTargets = 4;

public:
	static IFlashDisplayObject* context; //TODO:: Thread local

public:
	FQuerySelector():
		numTargets(0),
		targets(inplace),
		allocated(InplaceTargets)
	{
	};

	virtual ~FQuerySelector()
	{
		for ( int l_i = 0; l_i < numTargets; ++l_i )
		{
			GC_REMOVE_REF(targets[l_i]);
		};

		if ( targets != inplace )
		{
			delete[] targets;
		};
	};

	FQuerySelector(const FQuerySelector& _ref):
		numTargets(0),
		targets(inplace),
		allocated(InplaceTargets)
	{
		if ( allocated < _ref.allocated )
		{
			__reallocate(_ref.allocated);
		}

		numTargets = _ref.numTargets;

		for ( int i = 0; i < _ref.numTargets; ++i )
		{
			targets[i] = _ref.targets[i];
			GC_ADD_REF(targets[i]);
		};
	};

	FQuerySelector& operator=(const FQuerySelector& _ref)
	{
		if ( allocated < _ref.allocated )
		{
			__reallocate(_ref.allocated);
		};

		for ( int l_i = 0; l_i < numTargets; ++l_i)
		{
			GC_REMOVE_REF(targets[l_i]);
		};

		numTargets = _ref.numTargets;

		for ( int ll_i = 0; ll_i < _ref.numTargets; ++ll_i)
		{
			targets[ll_i] = _ref.targets[ll_i];
			GC_ADD_REF(targets[ll_i]);
		};

		return *this;
	}

	/*
	* Добавляет объект t в селектор
	*/
	FQuerySelector& add(IFlashDisplayObject* _target)
	{
		if ( _target != NULL )
		{
			if ( allocated <= numTargets )
			{
				__reallocate(allocated * 2 + 1);
			};

			targets[numTargets++] = _target;
			GC_ADD_REF(_target);
		};

		return *this;
	};

	/*
	* Добавляет объект t в контейнер. Если T - строка, создает и добавляет во все выбранные контейнеры
	* символ с заданным идентификатором.
	*/
	FQuerySelector append(IFlashDisplayObject* _target)
	{
		GC_STACKREF(_target);

		FQuerySelector newSelector;
		IPrototype* l_targetProto = _target->Prototype();
		Assert2(l_targetProto != NULL || numTargets <= 1, "can't append object to multiple targets");

		for ( int i = 0; i < numTargets; i++ )
		{
			IFlashDisplayObject* l_newTarget = i?l_targetProto->Clone()->DisplayObject():_target;
			IFlashSprite* l_sprite = targets[i]->Sprite();

			if (!l_sprite )
			{
				l_sprite = FQuerySelector().add(targets[i]).wrap().get()->Sprite();
			};

			l_sprite->addChild(l_newTarget);
			newSelector.add(l_newTarget);
		};

		return newSelector;
	};

	FQuerySelector append(const std::string& _name)
	{
		IFlashDisplayObject* l_newDisplayObject = Flash::FlashResourceManager::instance->createItem(_name);
		Assert2(l_newDisplayObject!=NULL, "can't add not existen item");

		return append(l_newDisplayObject);
	};

	FQuerySelector append(const char *_name)
	{
		return append(std::string(_name));
	};

	FQuerySelector append(const FQuerySelector& _ref)
	{
		return append(_ref.get(0));
	};

	FQuerySelector& replace(IFlashDisplayObject* _target)
	{
		GC_STACKREF(_target);

		IPrototype* l_targetProto = _target->Prototype();
		Assert2(l_targetProto != NULL || numTargets <= 1, "can't replace object in multiple targets");
		int j = 0;

		for(int l_i = 0; l_i < numTargets; ++l_i)
		{
			IFlashDisplayObject* l_oldTarget = targets[l_i];
			IFlashSprite* l_parent = l_oldTarget->getParent()->Sprite();

			if(l_parent)
			{
				IFlashDisplayObject* l_newTarget = j?l_targetProto->Clone()->DisplayObject():_target;

				l_parent->addChildAt(l_newTarget, l_parent->getChildIndex(l_oldTarget));
				l_parent->removeChild(l_oldTarget);

				l_newTarget->applyTransform(l_oldTarget);
				l_newTarget->setName(l_oldTarget->getName());

				GC_ADD_REF(l_newTarget);
				targets[j++] = l_newTarget;
			};

			GC_REMOVE_REF(l_oldTarget);
		};

		numTargets = j;

		return *this;
	};

	FQuerySelector& replace(const std::string& _name)
	{
		IFlashDisplayObject* l_newDisplayObject = Flash::FlashResourceManager::instance->createItem(_name);
		Assert2(l_newDisplayObject!=NULL, "can't replace to non existen item");

		return replace(l_newDisplayObject);
	};

	FQuerySelector& replace(const char* name)
	{
		return replace(std::string(name));
	}

	FQuerySelector& replace(const FQuerySelector& _ref)
	{
		return replace(_ref.get(0));
	};

	FQuerySelector& empty()
	{
		for ( int i = 0; i < numTargets; i++ )
		{
			IFlashSprite* parent = targets[i]->Sprite();

			if ( parent )
			{
				while ( parent->getChildrenCount() > 0 )
				{
					parent->removeChildAt(parent->getChildrenCount() - 1);
				};
			};
		};

		return *this;
	}

	FQuerySelector& detach(){
		for ( int i = 0; i < numTargets; i++ ){
			IFlashDisplayObject* parent = targets[i]->getParent();
			if ( parent ){
				IFlashSprite* sprite = parent->Sprite();
				if ( sprite ){
					sprite->removeChild(targets[i]);
				}
			}
		}
		return *this;
	}

	/*
	* Возвращает элемент из селектора по индексу
	*/
	IFlashDisplayObject* get(int _index = 0) const
	{
		Assert2( 0 <= _index && _index < numTargets, "index out of bounds");
		return targets[_index];
	};

	IFlashDisplayObject* maybeGet(int _index = 0) const
	{
		if (0 <= _index && _index < numTargets)
		{
			return targets[_index];
		};

		return NULL;
	};

	IFlashDisplayObject* operator[](int _index) const
	{
		if (0 <= _index && _index < numTargets)
		{
			return targets[_index];
		};

		return NULL;
	};

	/**
	* Возвращает число элементов селектора
	*/
	int size() const
	{
		return numTargets;
	};

	/*
	* Оборачивает объекты селектора в контейнеры и возвращает новый селектор, содержащий эти
	* контейнеры
	*/
	FQuerySelector wrap(){
		FQuerySelector newSelector;
		for ( int i = 0; i < numTargets; i++ ){
			IFlashSprite* sprite = new CPrototype<FlashSprite>();
			IFlashDisplayObject* parent = targets[i]->getParent();
			IFlashSprite* parentSprite = parent->Sprite();
			if ( !parentSprite ){
				Halt("parent is not a sprite - can't wrap");
				return newSelector;
			}
			int index = parentSprite->getChildIndex(targets[i]);
			parentSprite->removeChildAt(index);
			parentSprite->addChildAt(targets[i], index);
			sprite->applyTransform(targets[i]);
			float identity[] = {
				1, 0, 0,
				0, 1, 0
			};
			targets[i]->setMatrix(identity);
			targets[i]->setColor(Color::WHITE);
			newSelector.add(sprite);
		}
		return newSelector;
	}

		/*
		* Выбирает дочерние элементы объектов селектора
		*/
		FQuerySelector children(){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashSprite* sprite = targets[i]->Sprite();
				if ( sprite ){
					for ( int j = 0; j < sprite->getChildrenCount(); j++ ){
						newSelector.add(sprite->getChildAt(j));
					}
				}
			}
			return newSelector;
		}

		/**
		* Возвращает родительские элементы объектов селектора
		*/
		FQuerySelector parent(){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* newTarget = targets[i]->getParent();
				if ( newTarget ){
					newSelector.add(newTarget);
				}
			}
			return newSelector;
		}

		/**
		* Убирает из селектора повторяющиеся элементы
		*/
		FQuerySelector& unique(){
			int k = 0;
			for ( int i = 0; i < numTargets; i++ ){
				GC_REMOVE_REF(targets[i]);
				int j;
				for ( j = 0; j < k; j++ ){
					if ( targets[j] == targets[i] ){
						break;
					}
				}
				if ( j >= k ){
					GC_ADD_REF(targets[i]);
					targets[k] = targets[i];
					k++;
				}
			}
			numTargets = k;
			return *this;
		}

		static IFlashDisplayObject* __getChildByName(IFlashDisplayObject* thisObject, const std::string& name)
		{
			IFlashSprite* l_sprite = thisObject->Sprite();

			if ( l_sprite)
			{
				return l_sprite->getChildByName(name);
			};

			return NULL;
		}

		static IFlashDisplayObject* __queryChild(IFlashDisplayObject* thisObject, const std::string& name)
		{
			IFlashDisplayObject* displayObject = thisObject;

			int n = (int)name.size();
			int j = 0;
			for ( int i = 0; i <= n; i++ ){
				if ( i == n || name[i] == '.' ){
					displayObject = __getChildByName(displayObject, name.substr(j, i - j));
					j = i + 1;
					if ( !displayObject )
						return 0;
				}
			}

			return displayObject;
		};

		/**
		* Выбирает дочерние элементы по имени
		*/
		FQuerySelector children(const std::string& name) const
		{
			FQuerySelector newSelector;

			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* newTarget = __queryChild(targets[i], name);
				if ( newTarget ){
					newSelector.add(newTarget);
				}
			}

			return newSelector;
		}

		FQuerySelector children(const std::string& name1, const std::string& name2){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* newTarget;

				newTarget = __queryChild(targets[i], name1);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name2);
				if ( newTarget ){
					newSelector.add(newTarget);
				}
			}
			return newSelector;
		}

		FQuerySelector children(const std::string& name1, const std::string& name2, const std::string& name3){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* newTarget;

				newTarget = __queryChild(targets[i], name1);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name2);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name3);
				if ( newTarget ){
					newSelector.add(newTarget);
				}
			}
			return newSelector;
		}

		FQuerySelector children(const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* newTarget;

				newTarget = __queryChild(targets[i], name1);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name2);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name3);
				if ( newTarget ){
					newSelector.add(newTarget);
				}

				newTarget = __queryChild(targets[i], name4);
				if ( newTarget ){
					newSelector.add(newTarget);
				}
			}
			return newSelector;
		}

		template<class It>
		FQuerySelector children(It begin, It end){
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				It it = begin;
				for ( It it = begin; it != end; it++ ){
					IFlashDisplayObject* newTarget = __queryChild(targets[i], *it);
					if ( newTarget ){
						newSelector.add(newTarget);
					}
				}
			}
			return newSelector;
		}

		/**
		* Применяет функцию ко всем объектам селектора
		*/
		template<class F>
		FQuerySelector& apply(F f){
			IFlashDisplayObject* savedContext = context;
			for ( int i = 0; i < numTargets; i++ ){
				context = targets[i];
				f(targets[i]);
			}
			context = savedContext;
			return *this;
		}

		template<class F>
		FQuerySelector& with(F f){
			FQuerySelector copy(*this);
			f(copy);
			return *this;
		}

		/**
		* Применяет фильтр к объектам селектора
		*/
		template<class F>
		FQuerySelector& filter(F f){
			int j = 0;
			for ( int i = 0; i < numTargets; i++ ){
				if ( !(bool)f(targets[i]) ){
					GC_REMOVE_REF(targets[i]);
				}else{
					targets[j] = targets[i];
					j++;
				}
			}
			numTargets = j;
			return *this;
		}

		/**
		* Находит ближайший объект в иерархии, удовлетворяющий условию
		*/
		template<class F>
		FQuerySelector closest(F f) const {
			FQuerySelector newSelector;
			for ( int i = 0; i < numTargets; i++ ){
				IFlashDisplayObject* child = targets[i];
				while ( child ){
					if ( (bool)f(child) ){
						newSelector.add(child);
						break;
					}
					child = child->getParent();
				}
			}
			return newSelector;
		}

		/**
		* Очищает селектор
		*/
		FQuerySelector& clear() {
			for ( int i = 0; i < numTargets; i++ ){
				GC_REMOVE_REF(targets[i]);
			}
			numTargets = 0;
			return *this;
		}

		/**
		* Устанавливает обработчик события
		*/
		FQuerySelector& bind(const std::string& event, EventHandler handler, bool capture = false){
			for ( int i = 0; i < numTargets; i++ ){
				EventManager::Get().addEventListener(targets[i], event, handler, capture);
			}
			return *this;
		}

		/**
		* Снимает обработчик события
		*/
		FQuerySelector& unbind(const std::string& event, EventHandler handler){
			for ( int i = 0; i < numTargets; i++ ){
				EventManager::Get().removeEventListener(targets[i], event, handler);
			}
			return *this;
		}

		/**
		* Снимает все обработчики событий
		*/
		FQuerySelector& unbindAll(){
			for ( int i = 0; i < numTargets; i++ ){
				EventManager::Get().removeEventListeners(targets[i]);
			}
			return *this;
		}


		/**
		* Вписывает объект в заданный прямоугольник
		*/
		FQuerySelector& fit(float left, float top, float right, float bottom){
			for ( int i = 0; i < numTargets; i++ ){
				float l, t, r, b;
				targets[i]->getBounds(l, t, r, b, targets[i]->getParent());
				float w = r - l;
				float h = b - t;
				float scaleX = (right - left) / w;
				float scaleY = (bottom - top) / h;
				float scale = std::min(scaleX, scaleY);
				targets[i]->getScale(scaleX, scaleY);
				targets[i]->setScale(scaleX * scale, scaleY * scale);
				targets[i]->getBounds(l, t, r, b, targets[i]->getParent());
				float x, y;
				targets[i]->getPosition(x, y);
				float dw = ((right - left) - (r - l)) * 0.5f;
				float dh = ((bottom - top) - (b - t)) * 0.5f;
				targets[i]->setPosition(x + (left - l) + dw, y + (top - t) + dh);
			}
			return *this;
		}

		/**
		* Отправляет событие объектам
		*/
		FQuerySelector& send(FlashEvent& event){
			for ( int i = 0; i < numTargets; i++ ){
				EventManager::Get().dispatch(targets[i], event);
			}
			return *this;
		}

		/**
		* Устанавливает текст для текстового поля
		*/
		FQuerySelector& text(const std::string& text){
			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->Text() ){
					targets[i]->Text()->setText(text);
				}
			}
			return *this;
		}

		/**
		* Возвращает текст объекта
		*/
		const std::string& text(int index = 0) const {
			IFlashDisplayObject* object = get(index);
			if ( !object->Text() ){
				Halt("not a text field");
				static std::string empty("");
				return empty;
			}
			return object->Text()->getText();
		}

		/**
		* Устанавливает имя объекта
		*/
		FQuerySelector& name(const std::string& name){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setName(name);
			}
			return *this;
		}

		/**
		* Возвращает имя объекта
		*/
		const std::string& name(int index = 0) const {
			IFlashDisplayObject* object = get(index);
			return object->getName();
		}

		/**
		* Получает фактические границы текста в текстовом поле
		*/
		FQuerySelector& textBounds(float &left, float &top, float &right, float &bottom){
			IFlashText* text = get(0)->Text();
			if ( text ){
				Flash::getTextFieldContentMetrics(text, left, top, right, bottom);
			}else{
				bounds(left, top, right, bottom);
			}
			return *this;
		}

		/**
		* Получает фактические размеры текста в текстовом поле
		*/
		FQuerySelector& textSize(float &width, float &height){
			float left, top, right, bottom;
			textBounds(left, top, right, bottom);
			width = right - left;
			height = bottom - top;
			return *this;
		}

		/**
		* Получает границы объекта
		*/
		FQuerySelector& bounds(IFlashDisplayObject* target, float &left, float &top, float &right, float &bottom){
			get(0)->getBounds(left, top, right, bottom, target);
			return *this;
		}

		/**
		* Получает границы объекта
		*/
		FQuerySelector& bounds(float &left, float &top, float &right, float &bottom){
			get(0)->getBounds(left, top, right, bottom, 0);
			return *this;
		}

		Vec2 position(int index = 0) const {
			float x, y;
			get(index)->getPosition(x, y);
			return Vec2(x, y);
		}

		FQuerySelector& position(Vec2 vec){
			return position(vec.x, vec.y);
		}

		FQuerySelector& position(float x, float y){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setPosition(x, y);
			}
			return *this;
		}

		FQuerySelector& shift(float dx, float dy){
			for ( int i = 0; i < numTargets; i++ ){
				float x, y;
				targets[i]->getPosition(x, y);
				targets[i]->setPosition(x + dx, y + dy);
			}
			return *this;
		}

		FQuerySelector& zoom(float k){
			for ( int i = 0; i < numTargets; i++ ){
				float x, y;
				targets[i]->getScale(x, y);
				targets[i]->setScale(x * k, y * k);
			}
			return *this;
		}

		Vec2 scale(int index = 0) const {
			float x, y;
			get(index)->getScale(x, y);
			return Vec2(x, y);
		}

		FQuerySelector& scale(Vec2 vec){
			return scale(vec.x, vec.y);
		}

		FQuerySelector& scale(float x, float y){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setScale(x, y);
			}
			return *this;
		}

		float rotation(int index = 0) const {
			return get(index)->getRotation();
		}

		FQuerySelector& rotation(float x){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setRotation(x);
			}
			return *this;
		}

		float skew(int index = 0) const {
			return get(index)->getShear();
		}

		FQuerySelector& skew(float x){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setShear(x);
			}
			return *this;
		}

		float alpha(int index = 0) const {
			return get(index)->getAlpha();
		}

		FQuerySelector& alpha(float x){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setAlpha(x);
			}
			return *this;
		}

		FQuerySelector& show(bool flag = true){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setVisible(flag);
			}
			return *this;
		}

		FQuerySelector& hide(bool flag = true){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setVisible(!flag);
			}
			return *this;
		}

		float rate() const {
			return get()->getUpdateRate();
		}

		FQuerySelector& rate(float x){
			for ( int i = 0; i < numTargets; i++ ){
				targets[i]->setUpdateRate(x);
			}
			return *this;
		}

		LocalStorage* data(int index = 0){
			if ( !storage )
			{
				CGCRef<LocalStorageDictionary>* l_storage = new CGCRef<LocalStorageDictionary>();
				storage = static_cast<LocalStorageDictionary*>(l_storage);

				GC_SHOULD_TRACK(l_storage);
				GC_ADD_REF(l_storage);
			}

			return storage->getStorage(get(index));
		}

		static void clearStorageFor(IFlashDisplayObject* target){
			if ( storage ){
				storage->removeStorage(target);
			}
		}

		/*
		movie clips
		*/
		int frames(){
			int frames = 1;
			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->MovieClip() ){
					frames = lcd(targets[i]->MovieClip()->countFrames(), frames);
				}
			}
			return frames;
		}

		FQuerySelector& play(bool state = true){
			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->MovieClip() ){
					targets[i]->MovieClip()->setPlayback(state);
				}
			}
			return *this;
		}

		FQuerySelector& stop(){
			return play(false);
		}

		int frame() const {
			IFlashMovieClip* movieClip = get(0)->MovieClip();
			if ( movieClip ){
				return movieClip->getCurrentFrame();
			}
			return 0;
		}

		FQuerySelector& frame(int frame){
			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->MovieClip() ){
					targets[i]->MovieClip()->gotoFrame(frame);
				}
			}
			return *this;
		}

		FQuerySelector& frame(const std::string& label){
			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->MovieClip() ){
					targets[i]->MovieClip()->gotoLabel(label);
				}
			}
			return *this;
		}

		/**
		* Активирует события воспроизведения для клипа (PlaybackFrame, PlaybackEnd, PlaybackRestarted)
		*/
		FQuerySelector& listenPlayback(){
			if ( !fQueryEventsPlaybackOperation ){
				fQueryEventsPlaybackOperation = new FQueryEventsPlaybackOperation();
			}

			for ( int i = 0; i < numTargets; i++ ){
				if ( targets[i]->MovieClip() ){
					targets[i]->MovieClip()->setPlaybackOperation(fQueryEventsPlaybackOperation);
				}
			}
			return *this;
		}

		FQuerySelector& disable(){
			FlashEvent event("disable");
			this->send(event);
			return *this;
		}

		FQuerySelector& enable(){
			FlashEvent event("enable");
			this->send(event);
			return *this;
		}

		FQuerySelector& check(){
			FlashEvent event("check");
			this->send(event);
			return *this;
		}

		FQuerySelector& uncheck(){
			FlashEvent event("uncheck");
			this->send(event);
			return *this;
		}

		FQuerySelector& value(bool value){
			ValueEvent<bool> event("setBoolValue", value);
			this->send(event);
			return *this;
		}

		FQuerySelector& value(double value){
			ValueEvent<double> event("setNumberValue", value);
			this->send(event);
			return *this;
		}

		bool queryBool(){
			QueryEvent<bool> event("queryBoolValue");
			this->send(event);
			return event.get();
		}

		double queryNumber(){
			QueryEvent<double> event("queryNumberValue");
			this->send(event);
			return event.get();
		}

		bool queryBool(bool def){
			QueryEvent<bool> event("queryBoolValue");
			this->send(event);
			return event.get(def);
		}

		double queryNumber(double n){
			QueryEvent<double> event("queryNumberValue");
			this->send(event);
			return event.get(n);
		}

		class iterator: public std::iterator<std::bidirectional_iterator_tag, IFlashDisplayObject*>{
		public:
			typedef IFlashDisplayObject* value_type;

			iterator(FQuerySelector* selector, int index):selector(selector), index(index){
			}

			bool operator==(iterator const& rhs) const
			{
				return (selector == rhs.selector) && (index == rhs.index || (invalid_index(index) && invalid_index(rhs.index)));
			}

			bool operator!=(iterator const& rhs) const
			{
				return !(*this==rhs);
			}

			iterator& operator++()
			{
				index++;
				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp (*this);
				++(*this);
				return tmp;
			}

			iterator& operator--()
			{
				if ( index >= 0 ){
					index--;
				}
				return *this;
			}

			iterator operator--(int)
			{
				iterator tmp (*this);
				--(*this);
				return tmp;
			}

			value_type operator* () const
			{
				return selector->get(index);
			}

		private:
			bool invalid_index(int index) const{
				return index < 0 || index >= selector->size();
			}

			FQuerySelector* selector;
			int index;
		};

		iterator begin(){
			return iterator(this, 0);
		}

		iterator end(){
			return iterator(this, -1);
		}

		static IFlashDisplayObject* pushContext(IFlashDisplayObject* context){
			IFlashDisplayObject* old = FQuerySelector::context;
			FQuerySelector::context = context;
			return old;
		}

		static void restoreContext(IFlashDisplayObject* context){
			FQuerySelector::context = context;
		}

private:
	void __reallocate(int _size)
	{
		IFlashDisplayObject** newTargets = new IFlashDisplayObject*[_size];
		memcpy(newTargets, targets, numTargets * sizeof(IFlashDisplayObject*));

		if ( targets != inplace )
		{
			delete[] targets;
		};

		allocated = _size;
		targets = newTargets;
	};

private:
	int numTargets, allocated;
	IFlashDisplayObject** targets = nullptr;
	IFlashDisplayObject* inplace[InplaceTargets] = { nullptr, nullptr, nullptr, nullptr};

private:
	static LocalStorageDictionary* storage;
	static FQueryEventsPlaybackOperation* fQueryEventsPlaybackOperation;
};

inline FQuerySelector fQuery(IFlashDisplayObject* t)
{
	return FQuerySelector().add(t);
};

inline FQuerySelector fQuery(const std::string& _sel, IFlashDisplayObject* _t)
{
	return FQuerySelector().add(_t).children(_sel);
};

inline FQuerySelector fQuery(const char* _sel, IFlashDisplayObject* _t)
{
	return fQuery(std::string(_sel), _t);
};

inline FQuerySelector fQuery(const std::string& _sel, const FQuerySelector& _t)
{
	return _t.children(_sel);
};

inline FQuerySelector fQuery(const char* sel, const FQuerySelector& t)
{
	return fQuery(std::string(sel), t);
};
};

/* RefCounter.h
 *
 * Вспомогательные классы для использования boost::intrusive_ptr.
 */

#ifndef __REFCOUNTER_H__
#define __REFCOUNTER_H__

#include <cstdint>

#include "EngineAssert.h"

#include <boost/intrusive_ptr.hpp>
#include "intrusive_weak_ptr.hpp"

namespace details {

class RefCounterBase
{
protected:
	RefCounterBase() : counter_(0), counter_weak_(0) {}
	explicit RefCounterBase(const RefCounterBase&) : counter_(0), counter_weak_(0) {}
	RefCounterBase& operator=(const RefCounterBase&) { return *this; }

	virtual ~RefCounterBase() {
		Assert(counter_ == 0);
		// counter_weak_ может быть не 0.
	}

public:
	uint16_t GetUseCount() const { return counter_; }
	uint16_t GetWeakCount() const { return counter_weak_; }

private:
	friend void intrusive_ptr_add_ref(RefCounterBase *p) {
		Assert(p->counter_ < 0xffff);
		++p->counter_;
	}

	friend void intrusive_ptr_release(RefCounterBase * p) {
		Assert(p->counter_ > 0);
		--p->counter_;
		if (0 == p->counter_) {
			if (0 == p->counter_weak_) {
				delete p; // Обычное удаление.
			} else {
				// Частичное удаление.
				void * mdp = dynamic_cast<void*>(p); // Находим указатель на начало объекта (p может указывать в середину объекта при множественном наследовании).
				p->~RefCounterBase(); // Вызываем деструктор (виртуальный), но не освобождаем память, т.к. она ещё используется под счётчик weak_ptr.
				*(void**)p = mdp; // Записываем указатель вместо vptr (всё равно он уже уничтожен).
			}
		}
	}

	friend void intrusive_weak_ptr_add_ref(RefCounterBase * p) {
		Assert(p->counter_weak_ < 0xffff);
		++p->counter_weak_;
	}

	friend void intrusive_weak_ptr_release(RefCounterBase * p) {
		Assert(p->counter_weak_ > 0);
		--p->counter_weak_;
		if (0 == p->counter_weak_) {
			if (0 == p->counter_) {
				void * mdp = *(void**)p; // Берём записанный в vptr указатель.
				::operator delete(mdp); // Освобождаем память.
			}
		}
	}

	friend bool intrusive_weak_ptr_alive(RefCounterBase * p) {
		return p->counter_ > 0;
	}

private:
	mutable uint16_t counter_ = 0; ///< Счётчик ссылок.
	mutable uint16_t counter_weak_ = 0; ///< Счётчик ссылок для слабых ссылок.
};

} // namespace details

/** Класс реализует подсчёт ссылок для boost::intrusive_ptr.
 * Использовать в качестве базового класса:
 * class MyClass : public RefCounter //< Наследование!
 * {
 *    ...
 * };
 *
 * typedef boost::intrusive_ptr<MyClass> MyClassRef; //< Не обязательно, но удобно.
 *
 * MyClassRef ref = p; // Делает intrusive_ptr из указателя на MyClass *с учётом всех ссылок*!
 *
 * Вместо intrusive_ptr предпочтительнее, если возможно, испрользовать std::unique_ptr (с++11).
 * Также к вашим услугам контейнеры для указателей в boost:
 * http://www.boost.org/doc/libs/1_55_0/libs/ptr_container/doc/ptr_container.html
 */
class RefCounter : virtual public details::RefCounterBase
{
};

#endif // __REFCOUNTER_H__

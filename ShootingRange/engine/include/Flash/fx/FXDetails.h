#pragma once

#include "RefCounter.h"

struct IFlashDisplayObject;

namespace fx {

	class FlashFXEffectCombinator: public RefCounter {
	public:
		virtual void apply(float time) = 0;
	};

	typedef boost::intrusive_ptr<FlashFXEffectCombinator> FlashFXEffect;

	class FlashFXPropertyCombinator: public RefCounter {
	public:
		virtual void set(float value) = 0;
		virtual float get() = 0;
	};

	typedef boost::intrusive_ptr<FlashFXPropertyCombinator> FlashFXProperty;

	class FlashFXCombinator;
	typedef boost::intrusive_ptr<FlashFXCombinator> FlashFX;

	class FlashFXCombinator: public RefCounter {
	public:
		virtual FlashFX update(float& dt) = 0;
		virtual FlashFX discard() = 0;
		virtual FlashFX clone() = 0;
	};

	FlashFXProperty operator^(IFlashDisplayObject*, const std::string& s);

	class FXList {
	public:
		FXList();
		~FXList();
		int addFx(FlashFX fx);
		void discardFx(int fx);
		void killFx(int fx);
		void discardFxAll();
		void killFxAll();
		void update(float dt);

	private:
		int nextIndex;
		typedef std::pair<int, FlashFX> EffectPair;
		typedef std::list<EffectPair> EffectList;
		EffectList fxs;
		EffectList pending;
		bool updating;
	};

	class FXInterpolationBase: public RefCounter {
	public:
		FXInterpolationBase(void* data, void (*deleter)(void*), float (*functor)(void*, float)) {
			this->data = data;
			this->functor = functor;
			this->deleter = deleter;
		}

		~FXInterpolationBase() {
			deleter(data);
		}

		float eval(float t) {
			return functor(data, t);
		}
	private:
		void* data;
		void (*deleter)(void*);
		float (*functor)(void*, float);
	};

	typedef boost::intrusive_ptr<FXInterpolationBase> FXInterp;

	template<typename Functor>
	void functor_deleter(void* f) {
		Functor *func = (Functor*)f;
		delete func;
	}
	template<typename Functor>
	float functor_interp(void* f, float t) {
		Functor* closure = (Functor*)f;
		return (*closure)(t);
	}

	template<typename Functor>
	FXInterp finterp() {
		return new FXInterpolationBase(new Functor(), functor_deleter<Functor>, functor_interp<Functor>);
	}

	template<typename Functor, typename A0>
	FXInterp finterp(A0 a0) {
		return new FXInterpolationBase(new Functor(a0), functor_deleter<Functor>, functor_interp<Functor>);
	}

	template<typename Functor, typename A0, typename A1>
	FXInterp finterp(A0 a0, A1 a1) {
		return new FXInterpolationBase(new Functor(a0, a1), functor_deleter<Functor>, functor_interp<Functor>);
	}

	template<typename Functor, typename A0, typename A1, typename A2>
	FXInterp finterp(A0 a0, A1 a1, A2 a2) {
		return new FXInterpolationBase(new Functor(a0, a1, a2), functor_deleter<Functor>, functor_interp<Functor>);
	}

	template<typename Functor, typename A0, typename A1, typename A2, typename A3>
	FXInterp finterp(A0 a0, A1 a1, A2 a2, A3 a3) {
		return new FXInterpolationBase(new Functor(a0, a1, a2, a3), functor_deleter<Functor>, functor_interp<Functor>);
	}

} // namespace fx

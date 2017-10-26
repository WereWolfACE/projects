#if !defined(_FLASH_FX_H_)
#define _FLASH_FX_H_

#pragma once

#include "FXDetails.h"

namespace fx {

	///Добавляет эффект в глобальный список эффектов
	int addGlobalEffect(FlashFX fx);
	///Удаляет эффект из глобального списка по его номеру
	void discardGlobalEffect(int fx);
	void killGlobalEffect(int fx);
	///Обновляет глобальный список эффектов
	void updateGlobalEffects(float dt);
	///Удаляет все эффекты из глобального списка
	void discardAllGlobalEffects();
	void killAllGlobalEffects();

	//Базовые комбинаторы анимации
	extern FlashFX zero;
	extern FlashFX one;

	///ждать t секунд, выполняя действие
	FlashFX wait(FlashFX, float t); 
	///повторять действие
	FlashFX fix(FlashFX);
	///выполнять как максимум t секунд
	FlashFX atmost(FlashFX, float t); 
	///выполнить подряд
	FlashFX seq(FlashFX, FlashFX);
	///выполнить вместе
	FlashFX par(FlashFX, FlashFX); 
	///интерполировать эффект за t секунд
	FlashFX effect(FlashFXEffect, float t); 
	///убрать из списка отображения объект
	FlashFX discard(IFlashDisplayObject*); 

	//выполнить эффект для заданного объекта (если объект пропадает со сцены - эффект будет потерян)
	FlashFX with(IFlashDisplayObject*, FlashFX);
	FlashFX withLayer(const std::string&, FlashFX);

	//Свойства объекта списка отображения

	FlashFXProperty x(IFlashDisplayObject*);
	FlashFXProperty y(IFlashDisplayObject*);
	FlashFXProperty scaleX(IFlashDisplayObject*);
	FlashFXProperty scaleY(IFlashDisplayObject*);
	FlashFXProperty rotation(IFlashDisplayObject*);
	FlashFXProperty alpha(IFlashDisplayObject*);
	FlashFXProperty scale(IFlashDisplayObject*);
	FlashFXProperty customBoxStartX(IFlashDisplayObject*);
	FlashFXProperty customBoxEndX(IFlashDisplayObject*);
	FlashFXProperty customBoxStartY(IFlashDisplayObject*);
	FlashFXProperty customBoxEndY(IFlashDisplayObject*);
	FlashFXProperty colorR(IFlashDisplayObject*);
	FlashFXProperty colorG(IFlashDisplayObject*);
	FlashFXProperty colorB(IFlashDisplayObject*);

	///объединить два эффекта
	FlashFXEffect unify(FlashFXEffect, FlashFXEffect);
	///интерполировать свойство установкой его значения
	FlashFXEffect tween(FlashFXProperty, FXInterp);
	///интерполировать свойство добавлением к нему значения
	FlashFXEffect add(FlashFXProperty, FXInterp);
	///интерполировать свойство умножением на значение
	FlashFXEffect mult(FlashFXProperty, FXInterp);
	///интерполировать свойство значением по маске
	FlashFXEffect blend(FlashFXProperty, FXInterp, FXInterp);

	///интерполяция константы
	FXInterp constant(float a);
	///линейная интерполяция
	FXInterp linear(float a, float b);
	///квадратичная интерполяция
	FXInterp quad(float a, float b, float c);
	///easing
	FXInterp ease(FXInterp, FXInterp);

	FXInterp mult_interp(FXInterp, FXInterp);
	FXInterp sum_interp(FXInterp, FXInterp);
	FXInterp subtract_interp(FXInterp, FXInterp);

	namespace ext {

		inline FXInterp operator*(FXInterp left, FXInterp right){
			return mult_interp(left, right);
		}
		inline FXInterp operator+(FXInterp left, FXInterp right){
			return sum_interp(left, right);
		}
		inline FXInterp operator-(FXInterp left, FXInterp right){
			return subtract_interp(left, right);
		}

		inline FlashFXEffect operator+(FlashFXEffect left, FlashFXEffect right){
			return unify(left, right);
		}

		inline FlashFX operator>>(FlashFX left, FlashFX right){
			return seq(left, right);
		}

		inline FlashFX operator+(FlashFX left, FlashFX right){
			return par(left, right);
		}

		inline FlashFX operator|(FlashFXEffect fx, float time){
			return effect(fx, time);
		}

		inline FlashFX operator|(FlashFX fx, float time){
			return wait(fx, time);
		}

		inline FlashFX operator||(FlashFX fx, float time){
			return atmost(fx, time);
		}

		inline FlashFX instant(FlashFXEffect fx){
			return fx | 0.0f;
		}

		inline FlashFX set(FlashFXProperty prop, float value){
			return tween(prop, constant(value)) | 0.0f;
		}

		inline FlashFX  operator<<(FlashFXProperty prop, float value){
			return set(prop, value);
		}

		inline FlashFX sleep(float time){
			return one | time;
		}

		inline FlashFX fixWith(FlashFX fx, IFlashDisplayObject* target){
			return with(target, fix(fx));
		}

		inline FlashFXEffect blendLinear(FlashFXProperty prop, float value) { return blend(prop, constant(value), linear(0.0f, 1.0f)); }
		inline FlashFXEffect renderFromAlpha(IFlashDisplayObject* object)   { return blend(alpha(object), linear(1.0f, 1.0f), linear(0.0f, 1.0f)); }
		inline FlashFXEffect fadeToAlpha(IFlashDisplayObject* object)       { return mult(alpha(object), linear(1.0f, 0.0f)); }
	
	} // namespace ext
} // namespace fx

#endif

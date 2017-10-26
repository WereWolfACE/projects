#ifndef __BINDABLE_H__
#define __BINDABLE_H__

#pragma once

#include "Render/RenderTypes.h"

namespace Render {

class RenderDeviceGLES2;

///
/// Базовый класс для объектов (текстур), которые связываются с устройством.
///
class Bindable : public RefCounter {
public:
	Bindable()
		: _filtering(FilteringType::Bilinear)
		, _address(AddressMode::Clamp)
		, _filteringDirty(true)
		, _addressDirty(true)
	{
	}

	virtual ~Bindable() = default;

	/// Возвращает тип фильтрации
	FilteringType GetFilteringType() const { return _filtering; }

	/// Устанавливает тип фильтрации
	void SetFilteringType(FilteringType filtering) { _filtering = filtering; _filteringDirty = true; }

	/// Возвращает режим адресации
	AddressMode GetAddressMode() const { return _address; }

	/// Устанавливает режим адресации
	void SetAddressMode(AddressMode address) { _address = address; _addressDirty = true; }

	/// Проверяет готовность объекта к использованию
	virtual bool Empty() const = 0;

	/// Связывает объект с графическим устройством
	virtual void Bind(int channel, int stageOp) = 0;

	/// Возвращает идентификатор объекта в графическом устройстве
	virtual const void* GetNativeHandle() const = 0;

	/// Возвращает тип объекта
	virtual TextureType GetTextureType() const = 0;

private:
	friend class RenderDeviceGLES2;

	FilteringType _filtering;   ///< Тип фильтрации
	AddressMode _address;       ///< Режим адресации

	bool _filteringDirty;    ///< Изменился ли _filtering
	bool _addressDirty;	  ///< Изменился ли _address
};

typedef boost::intrusive_ptr<Bindable> BindablePtr;

} // namespace Render

#endif // __BINDABLE_H__

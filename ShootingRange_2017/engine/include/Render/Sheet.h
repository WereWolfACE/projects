#ifndef __SHEET_H__
#define __SHEET_H__

#pragma once

#include "Render/PartialTexture.h"
#include "Render/Sprite.h"
#include "Core/Resource.h"

#ifdef ENGINE_TARGET_IPHONE
#include <strings.h>
#endif

namespace Render {

	/// Класс упакованного набора текстур
	/// Это класс "простыни" - большой, склееной из нескольких текстур.
	/// В классе хранятся координаты исходных тестур, и существует генерация псевдотекстур,
	/// которые могут отрисовываться из исходной простыни
	class Sheet : public Resource {
	public:
		/// Описание элемента на простыне
		struct SheetItem {
			int x, y, width, height, innerX, innerY, frameWidth, frameHeight, alphaLimit;
			TexturePtr tex;
			SpritePtr sprite;
		};

		/// генерация простыни из xml
		static Sheet* CreateFromXml(rapidxml::xml_node<>* elem);
		
		/// ручная загрузка простыни
		static Sheet* LoadSheet(const std::string& xml, const std::string& filename, const std::string& group = "", EnginePixelType pixelType = EnginePixelType::Default, bool useDithering = true, int alphaLimit = 0);

		/// Возвращает имя ресурса
		const std::string& GetName() const { return _path; }

		size_t GetMemoryInUse() const;

		void Load(ResourceLoadMode load_mode);
		void Unload(ResourceLoadMode load_mode);
		void GroupLoad(ResourceLoadManager& glm);
		void GroupUnload(ResourceLoadManager& glm);
		
		/// Проверка на существование текстуры с указанным именем файла
		/// (название файла должно совпадать полностью, т.е. с путём, но расширение не учитывается)
		bool TextureExists(const std::string& filename) const;
        
        bool SpriteExists(const std::string& filename) const;

		/// Заполняет структуру с описанием элемента в атласе.
		/// Если элемент не найден, возвращает false.
		bool GetSheetItem(const std::string& filename, SheetItem& item) const;
		
		/// Генерация псевдотекстуры по заданному файлу. До вызова должна быть произведена проверка
		/// наличия такой текстуры (см. CheckTexture). Если такой текстуры нет, возвращается NULL.
		/// В результате создаётся новый экземпляр Render::PartialTexture,
		/// его _нужно удалять самостоятельно_.
		Texture* CreateTexture(const std::string& filename, float prescale_x, float prescale_y) const;
        
        Sprite* CreateSprite(const std::string& filename) const;
		
		Texture* GetCachedTexture(int i) const;
		
		/// Возвращает количество текстур, упакованных в простыню
		int GetItemsCount() const { return _items.size(); }
		
		/// Возвращает текстуру простыни
		Texture* GetSheetTexture() const { return _tex.get(); }

	private:
		/// конструктор закрытый, ибо создавать объект только через getFromXml.
		Sheet();

		/// прочитать xml файл, описывающий содержащиеся в нём текстуры
		void ProcessXml(const std::string& xmlFile);

		Texture* CreateTexture(const SheetItem& it, float prescale_x, float prescale_y) const;

	private:
		/// Файл с текстурой
		std::string _path;

		/// Текстура простыни
		TexturePtr _tex;

		typedef std::map<std::string, SheetItem> SheetItems;
		/// Элементы атласа
		SheetItems _items;

		typedef std::vector<SheetItem*> OrderedItems;
		/// Кэшированные текстуры атласа
		mutable OrderedItems _ordered_items;
	};

	typedef boost::intrusive_ptr<Sheet> SheetPtr;
}

#endif // __SHEET_H__

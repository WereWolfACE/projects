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

	/// ����� ������������ ������ �������
	/// ��� ����� "��������" - �������, �������� �� ���������� �������.
	/// � ������ �������� ���������� �������� ������, � ���������� ��������� �������������,
	/// ������� ����� �������������� �� �������� ��������
	class Sheet : public Resource {
	public:
		/// �������� �������� �� ��������
		struct SheetItem {
			int x, y, width, height, innerX, innerY, frameWidth, frameHeight, alphaLimit;
			TexturePtr tex;
			SpritePtr sprite;
		};

		/// ��������� �������� �� xml
		static Sheet* CreateFromXml(rapidxml::xml_node<>* elem);
		
		/// ������ �������� ��������
		static Sheet* LoadSheet(const std::string& xml, const std::string& filename, const std::string& group = "", EnginePixelType pixelType = EnginePixelType::Default, bool useDithering = true, int alphaLimit = 0);

		/// ���������� ��� �������
		const std::string& GetName() const { return _path; }

		size_t GetMemoryInUse() const;

		void Load(ResourceLoadMode load_mode);
		void Unload(ResourceLoadMode load_mode);
		void GroupLoad(ResourceLoadManager& glm);
		void GroupUnload(ResourceLoadManager& glm);
		
		/// �������� �� ������������� �������� � ��������� ������ �����
		/// (�������� ����� ������ ��������� ���������, �.�. � ����, �� ���������� �� �����������)
		bool TextureExists(const std::string& filename) const;
        
        bool SpriteExists(const std::string& filename) const;

		/// ��������� ��������� � ��������� �������� � ������.
		/// ���� ������� �� ������, ���������� false.
		bool GetSheetItem(const std::string& filename, SheetItem& item) const;
		
		/// ��������� �������������� �� ��������� �����. �� ������ ������ ���� ����������� ��������
		/// ������� ����� �������� (��. CheckTexture). ���� ����� �������� ���, ������������ NULL.
		/// � ���������� �������� ����� ��������� Render::PartialTexture,
		/// ��� _����� ������� ��������������_.
		Texture* CreateTexture(const std::string& filename, float prescale_x, float prescale_y) const;
        
        Sprite* CreateSprite(const std::string& filename) const;
		
		Texture* GetCachedTexture(int i) const;
		
		/// ���������� ���������� �������, ����������� � ��������
		int GetItemsCount() const { return _items.size(); }
		
		/// ���������� �������� ��������
		Texture* GetSheetTexture() const { return _tex.get(); }

	private:
		/// ����������� ��������, ��� ��������� ������ ������ ����� getFromXml.
		Sheet();

		/// ��������� xml ����, ����������� ������������ � �� ��������
		void ProcessXml(const std::string& xmlFile);

		Texture* CreateTexture(const SheetItem& it, float prescale_x, float prescale_y) const;

	private:
		/// ���� � ���������
		std::string _path;

		/// �������� ��������
		TexturePtr _tex;

		typedef std::map<std::string, SheetItem> SheetItems;
		/// �������� ������
		SheetItems _items;

		typedef std::vector<SheetItem*> OrderedItems;
		/// ������������ �������� ������
		mutable OrderedItems _ordered_items;
	};

	typedef boost::intrusive_ptr<Sheet> SheetPtr;
}

#endif // __SHEET_H__

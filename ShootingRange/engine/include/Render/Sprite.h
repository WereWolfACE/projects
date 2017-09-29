#ifndef __SPRITE_H__
#define __SPRITE_H__

#pragma once

#include "Core/Resource.h"
#include "Core/ResourceFactory.h"
#include "Render/Drawable.h"
#include "Render/Texture.h"
#include "Render/SpriteBatch.h"
#include "Utils/IPoint.h"
#include "Utils/Xml.h"

namespace Render {
class Sheet;

class Sprite : public Resource, public Drawable {
public:
	Sprite();

	explicit Sprite(Texture* tex);

	virtual const Texture* GetTexture() const { return _texture.get(); }

	virtual const std::string& GetName() const override { return _id; }

	virtual size_t GetMemoryInUse() const override;

	virtual int Width() const override;

	virtual int Height() const override;

	virtual bool HitTest(int x, int y) const override;

	virtual void Draw(SpriteBatch* batch, const FPoint& position) override;

	virtual void GetGeometry(std::vector<QuadVert>& geometry, float z = 0, Color color = Color::WHITE) const override;

	using Drawable::HitTest;

	using Drawable::Draw;

	bool Empty() const;

	virtual void Load(ResourceLoadMode load_mode) override;

	virtual void Unload(ResourceLoadMode load_mode) override;

	virtual void GroupLoad(ResourceLoadManager& ldm) override;

	virtual void GroupUnload(ResourceLoadManager& ldm) override;

protected:
	friend void ResourceFactory::LoadResourcesXml(const std::string& filename);

	friend class Render::Sheet;

	void SetName(const std::string& name) { _id = name; }

	void SetTexture(Render::Texture* tex) { _texture = tex; }

protected:
	std::string _id;
	TexturePtr _texture;

private:
	Sprite(const Sprite&);
	Sprite& operator=(const Sprite&);
};

typedef boost::intrusive_ptr<Sprite> SpritePtr;


class PolygonalSprite : public Sprite {
public:
	PolygonalSprite(rapidxml::xml_node<>* xe, Texture* tex);

	virtual size_t GetMemoryInUse() const override;

	virtual int Width() const override;

	virtual int Height() const override;

	virtual bool HitTest(int x, int y) const override;

	virtual void Draw(SpriteBatch* batch, const FPoint& position) override;

	virtual void GetGeometry(std::vector<QuadVert>& geometry, float z = 0, Color color = Color::WHITE) const override;

protected:
	friend class Render::Sheet;

private:
	struct Triangle {
		FPoint pos[3];
		FPoint uv[3];
	};

	typedef std::vector<Triangle> Triangles;

	Triangles _triangles;

	IPoint _offset;

	int _width, _height;
};

typedef boost::intrusive_ptr<PolygonalSprite> PolygonalSpritePtr;

} // namespace Render

#endif // __SPRITE_H__

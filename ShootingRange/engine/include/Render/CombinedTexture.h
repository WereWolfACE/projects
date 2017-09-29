#ifndef _RENDER_COMBINEDTEXTURE_H_
#define _RENDER_COMBINEDTEXTURE_H_

#include "Render/Texture.h"

namespace Render {

class CombinedTexture : public Texture
{
public:
	CombinedTexture(const std::string& filename, const std::string& filename_pc);

	virtual size_t GetMemoryInUse() const override;

	/// Забиндить текстуру
	virtual void Bind(int channel = 0, int stageOp = 0) override;

	virtual bool isPixelTransparent(int x, int y) const override;

	virtual TextureType GetTextureType() const override { return TextureType::CombinedTexture; }

	Texture* GetColorTexture() const { return _colorTex.get(); }
	Texture* GetAlphaTexture() const { return _alphaTex.get(); }

protected:
	virtual void LoadFromFile(const std::string& filename = std::string()) override { Assert(false); }
	virtual void LoadFromImage(const Image& image_) override { Assert(false); }

	virtual void Load(ResourceLoadMode load_mode) override;
	virtual void Unload(ResourceLoadMode load_mode) override;
	virtual void GroupLoad(ResourceLoadManager& glm) override;
	virtual void GroupUnload(ResourceLoadManager& glm) override;

protected:
	TexturePtr _colorTex;
	TexturePtr _alphaTex;
};

} // namespace Render

#endif // _RENDER_COMBINEDTEXTURE_H_

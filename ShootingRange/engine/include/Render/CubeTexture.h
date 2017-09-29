#ifndef __CUBETEXTURE_H__
#define __CUBETEXTURE_H__

#pragma once

#include "Utils/FPoint.h"
#include "Utils/Math.hpp"
#include "Render/Texture.h"
#include "Core/Resource.h"

namespace Core
{
	class ResourceManager;
}

#if defined(ENGINE_TARGET_WIN32)
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#	define DEVCUBETEXTURE GLuint
#endif

#ifdef ENGINE_TARGET_IPHONE
#	include <OpenGLES/ES2/gl.h>
#	define DEVCUBETEXTURE GLuint
#endif

#if defined(ENGINE_TARGET_LINUX)
#	include <GLES/gl.h>
#	include <GLES/glext.h>
#	define DEVCUBETEXTURE GLuint
#endif

#ifdef ENGINE_TARGET_MACOS
#	include <OpenGL/OpenGL.h>
#	define DEVCUBETEXTURE GLuint
#endif

namespace Render
{
class RenderDeviceGLES2;

//  ласс кубической текстуры
class CubeTexture : public Resource, public Bindable
{
	friend class RenderDeviceGLES2;

public:
	std::string textureID;

	CubeTexture();
	virtual ~CubeTexture();

	void SetFaceFileName(int face, const std::string& path);
		
	static CubeTexture* CreateFromXml(rapidxml::xml_node<>* elem);
		
	virtual const std::string& GetName() const override { return textureID; }
	virtual size_t GetMemoryInUse() const override;

	/// “екстура не загружена, или принудительно выгружена. ѕринудительно выгружаютс€ динамические текстуры при переинициализации RenderDeviceImpl. —м. Texture::setDynamic
	bool Empty() const override;

	/// забиндить текстуру в заданный канал
	virtual void Bind(int channel = 0, int stageOp = 0) override;

	virtual const void* GetNativeHandle() const override { return reinterpret_cast<const void*>(_tex); }

	virtual TextureType GetTextureType() const override { return TextureType::CubeTexture; }

	/// загрузить текстуру
	virtual void Upload();
		/// выгрузить текстуру
	virtual void Release();
		
protected:

	//≈сли true, то paths задают пути к различным гран€м текстуры
	//в пор€дке -x, -y, -z, +x, +y, +z. ≈сли какой-то из paths пустой, то эта грань €вл€етс€ пустой
	//≈сли false, то paths[0] задает базовый путь к текстуре, который обрабатываетс€ движком
	//некоторым способом, завис€щим от реализации (не уверен, нужно ли кому-то данное поведение,
	//оставлено дл€ обратной совместимости)
	bool hasSeparatePaths;
	std::string paths[6];

	/// была ли текстура загружена
	bool _wasUploaded;

	DEVCUBETEXTURE _tex;
	EnginePixelType _pixelType;
};

typedef boost::intrusive_ptr<CubeTexture> CubeTexturePtr;
}

#endif // __CUBETEXTURE_H__

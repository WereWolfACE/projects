#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#pragma once

#include "Render/RenderTypes.h"
#include "RefCounter.h"
#include "RestorableObject.h"

#if defined(ENGINE_TARGET_IPHONE)
#include <OpenGLES/ES2/gl.h>
#elif defined(ENGINE_TARGET_WIN32) || defined(ENGINE_TARGET_LINUX)
#include <GLES2/gl2.h>
#elif defined(ENGINE_TARGET_MACOS)
#include <OpenGL/gl.h>
#else
#error "Unknown platform!"
#endif

#if defined(ENGINE_TARGET_IPHONE) || defined(ENGINE_TARGET_MACOS) || defined(ENGINE_TARGET_WIN32) || defined(ENGINE_TARGET_LINUX)
#define DEVINDEXBUFFER GLuint
#else
#error "Unknown platform!"
#endif

namespace Render {

class IndexBuffer : public RefCounter, public RestorableObject {
public:
	IndexBuffer();

	virtual ~IndexBuffer();

	void Create();

	void Destroy();

	bool IsValid() const;

	void SetData(size_t elements_count, const uint16_t* data = NULL, BufferUsage usage = BufferUsage::Static);

	void SetSubData(size_t elements_offset, size_t elements_count, const uint16_t* data);

	uint32_t GetSize() const { return _size; }

	void Bind() const;

	void Unbind() const;

private:
	virtual void OnContextLost() override;

private:
	DEVINDEXBUFFER _buffer;

	uint32_t _size;

private:
	// Копироание запрещено
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&);
};

} // namespace Render

#endif // __INDEXBUFFER_H__

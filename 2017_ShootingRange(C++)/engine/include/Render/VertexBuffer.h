#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

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
#define DEVVERTEXBUFFER GLuint
#else
#error "Unknown platform!"
#endif

namespace Render {

class VertexBuffer : public RestorableObject {
public:
	VertexBuffer();

	virtual ~VertexBuffer();

	void Create(const VertexElementDescription* decl);

	void Destroy();

	bool IsValid() const;

	void SetData(size_t elements_count, const void* pointer = NULL, BufferUsage usage = BufferUsage::Static);

	void SetSubData(size_t elements_offset, size_t elements_count, const void* data);

	uint32_t GetSize() const { return _size; }

	const VertexElementDescription* GetDecl() const { return _decl; }

	void Bind() const;

	void Unbind() const;

private:
	virtual void OnContextLost() override;
	
#if defined(ENGINE_TARGET_IPHONE)
	void WaitSync();
#endif

private:
	DEVVERTEXBUFFER _buffer;

	uint32_t _size;

	const VertexElementDescription* _decl;
	
#if defined(ENGINE_TARGET_IPHONE)
	GLsync _sync;
#endif

private:
	// Копироание запрещено
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&);
};

} // namespace Render

#endif // __VERTEXBUFFER_H__

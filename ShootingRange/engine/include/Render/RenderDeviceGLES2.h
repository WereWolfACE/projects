#ifndef _RENDERDEVICE_GL_ES2_H_
#define _RENDERDEVICE_GL_ES2_H_

#include "EngineAssert.h"
#include "Render/Texture.h"
#include "Render/CubeTexture.h"
#include "Render/RenderDeviceInterface.h"
#include "Render/RenderTypes.h"
#include "Render/VertexBufferBuilder.h"
#include "Render/Target.h"
#include "SceneGraph/Material.h"
#include "SceneGraph/Light.h"
#include "Utils/ClipSpace.h"
#include "Utils/Matrix4.h"
#include "Utils/FPoint.h"
#include "ClippingMode.h"

#if defined(ENGINE_TARGET_WIN32) || defined(ENGINE_TARGET_LINUX)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace Core
{
	class Window;
}

namespace Render
{

class Texture;
class ShaderProgram;
class ShaderProgramGLES2;
class VolumeTexture;
class VertexBufferBuilder;

class RenderDeviceGLES2 : public RenderDeviceInterface {
public:
	RenderDeviceGLES2();
	~RenderDeviceGLES2();

	virtual void BindWindowGL(void* layer, int contextWidth=0, int contextHeight=0);
	virtual void ClearDepthBuffer();
	virtual void Clear(const Color& c);
	virtual void SetDepthTest(bool bEnable);
	virtual void SetDepthWrite(bool bEnable);
	
	virtual void Upload(Texture* tex);
	virtual void Upload(Texture* tex, Image& image);
	virtual void UploadRegion(Texture* tex, int x, int y, int width, int height, const char* data);
	virtual void Upload(const Image& image, DEVTEXTURE& _tex, bool bilinear = true, bool clamp = true, const std::string& texId = "");
	virtual void Reload(Texture *tex);
	virtual void Bind(Texture* tex, int channel=0, unsigned int stageOperation = STAGE_DEFAULT);
	virtual void Release(Texture* tex);

	virtual void Upload(CubeTexture* tex);
	virtual void Bind(CubeTexture* tex, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT);
	virtual void Release(CubeTexture* tex);

	virtual void Upload(VolumeTexture* texture);
	virtual void Bind(VolumeTexture* texture, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT);
	virtual void Release(VolumeTexture*);
	
	virtual Target* CreateRenderTarget(int width, int height, const RenderTargetConfig& config = RenderTargetConfig());
	virtual void DeleteRenderTarget(Target*& tar);
	virtual void Bind(Target* tar, int channel=0, unsigned int stageOperation = STAGE_DEFAULT);
	virtual void Release(Target* tar);
	
	virtual void BeginRenderTo(Target* tar);
	using RenderDeviceInterface::BeginRenderTo;
	virtual void EndRenderTo();
	virtual void BeginRenderToTexture(Texture* texture);
	virtual void EndRenderToTexture();
	
	virtual void SetMaterial(SceneGraph::MaterialPtr pMaterial);
	
	virtual void SetViewFrustum(float left, float right, float bottom, float top, float zNear, float zFar);

	virtual void SetMatrixMode(MatrixMode mode);
    virtual MatrixMode GetMatrixMode() const { return _currentMatrixMode; }

    virtual void ResetMatrix();

	virtual const math::Matrix4& GetMatrix(MatrixMode matrix) const { return _currentMatrix[(int)matrix]; }
	virtual void SetMatrix(const math::Matrix4& matrix);
    virtual void MatrixMultiply(const math::Matrix4& matrix);
	virtual void MatrixTranslate(const math::Vector3& v);
	virtual void MatrixRotate(const math::Vector3& axis, float angle);
	virtual void MatrixScale(float scaleX, float scaleY, float scaleZ);
	virtual void PushMatrix();
	virtual void PopMatrix();

	virtual int GetStackSize(MatrixMode matrix);

	using RenderDeviceInterface::MatrixTranslate;
	using RenderDeviceInterface::MatrixScale;

	virtual void SetTexturing(bool bEnable);
	virtual bool GetTexturing() const;
	virtual void SetCulling(bool cullEnable);
	virtual void SetBlendMode(BlendMode blendMode);
	virtual void SetColorWriteMask(bool red, bool green, bool blue, bool alpha);
	virtual void SetCurrentColor(const Color& color = Color(0xff, 0xff, 0xff, 0xff));
	virtual Color GetCurrentColor() const;

	// Вывод геометрии
	//
	virtual void SetupGeometry(const VertexElementDescription* decl, const void* addr = NULL);
	virtual void SetupShader(const VertexElementDescription* decl, const void* addr = NULL);

	virtual void DrawPrimitives(PrimitiveType type, const VertexBuffer& vertices, size_t elements_offset, size_t elements_count);
	virtual void DrawIndexedPrimitives(PrimitiveType type, const VertexBuffer& vertices, const IndexBuffer& indices, size_t elements_offset, size_t elements_count);
	virtual void DrawPrimitives(PrimitiveType type, const void *vertices, const VertexElementDescription* decl, size_t elements_offset, size_t elements_count);
	virtual void DrawIndexedPrimitives(PrimitiveType type, const void *vertices, const VertexElementDescription* decl, const uint16_t* indices, size_t elements_offset, size_t elements_count);
	using RenderDeviceInterface::DrawPrimitives;
	using RenderDeviceInterface::DrawIndexedPrimitives;
	//
	//

	virtual void Begin2DMode();
	virtual void SetOrthoProjection(float width, float height, float minZ, float maxZ);
	virtual void Begin2DModeInViewport(int x, int y, int width, int height);
	virtual void ResetViewport();
    virtual void SetViewport(int x, int y, int width, int height);
    virtual IRect GetViewport();
	virtual void End2DMode();
	virtual void BeginScene();
	virtual void EndScene();
	virtual void Release();
	virtual void WriteVendorInfo();
	virtual int Width() const;
	virtual int Height() const;
	virtual FRect ScreenPos();
	virtual FRect ScreenPosPhysical();
	virtual int PhysicalWidth() const;
	virtual int PhysicalHeight() const;
	virtual bool LetterBoxScreen();
	virtual bool PillarBoxScreen();
	virtual void SetDefaultRenderState();
	virtual bool IsPower2Required();

	virtual void RegisterRestorableObject(RestorableObject* asset);
	virtual void UnregisterRestorableObject(RestorableObject* asset);

	virtual uint32_t GetVideoMemUsage() const;
	virtual void TranslateUV(FRect& rect, FRect& uv);
	virtual Texture* GetBindedTexture() const;
	virtual void SetBindedTexture(Texture* tex);
	virtual void BeginClipping(const IRect& window, ClippingMode clippingMode);	
	virtual void EndClipping();
	virtual void BeginClipPlane(float x1, float y1, float x2, float y2);
	virtual void EndClipPlane();
	virtual void CreateGLContext();
	virtual void DestroyGLContext();
	virtual void ClearBindedObject();
	virtual bool CanUseGPUSkinning(int bonesCount);

	virtual BlendMode GetBlendMode() const;
	virtual void GLFinish() const;
	
	virtual ShaderProgram *GetBindedShader();
	virtual void Bind(ShaderProgram* shader);
	virtual void Release(ShaderProgram* shader);
	virtual void ResetShader();
    virtual int GetMaxTextureUnits();
	virtual void SetPreferredMultisampleType(MultisampleType type) { _preferredMultisampleType = type; }
	virtual MultisampleType GetPreferredMultisampleType() const { return _preferredMultisampleType; }

	virtual void SetVSyncState(int intervals);
	virtual int GetVSyncState() const;
    
	virtual bool IsVertexShaderSupported(int majorVersion, int minorVersion);
	virtual bool IsPixelShaderSupported(int majorVersion, int minorVersion);
	virtual bool IsCubeTextureSupported();
	virtual bool IsVolumeTextureSupported();

	virtual void SetScissorTest(bool enable);

	virtual void SetStencilTest(bool enable);
	virtual void SetStencilFunc(StencilFunc func, int value, uint32_t mask = ~0);
	virtual void SetStencilOp(StencilOp fail, StencilOp zFail, StencilOp pass);

private:
	void CreateRenderTarget(Target* tar);

    void UpdateClipSpace(bool restore_buffers);

	/// Установить униформные (неизменяемые в течение одного рисования)
	/// переменные шейдера (матрица преобразования и номер текстурного канала)
	void SetUniforms();

	void SetTexParams(Bindable* tex);

	void SelectDrawMode(int channel, unsigned int stageOperation);

	void UseShaderProgram(ShaderProgramGLES2* program);

	GLint GetMultisamples(MultisampleType msType);

private:
	bool _windowBinded;

	void SetPresentParams(int w = 800, int h = 600);
	void SetRenderBufferSize(int width, int height);
	
#if defined(ENGINE_TARGET_WIN32) || defined(ENGINE_TARGET_LINUX)
	EGLDisplay _display;
	EGLContext _context;
	EGLSurface _surface;
	EGLConfig _config;
#endif

	//
	// Свойства материала
	//
	Color _currentColor;
	Texture* _bindedTexture; ///< Последняя забинденная текстура
	ShaderProgramGLES2* _specShader; ///< Шейдер, выбранный вручную.
	BlendMode _currentBlendMode;
	bool _currentCulling;
	bool _currentDepthTest;
	bool _currentDepthWrite;

	int _curTexChannel; ///< Выбранный текстурный канал
	
	bool _areUniformsUpToDate;

    Target* _currentRenderTarget; // Активный таргет, в который происходит рендеринг

	ShaderType _drawMode; ///< Используется для выбора шейдера.
	ShaderProgramGLES2* _currentShader; ///< Текущий забинденный шейдер.

	/// текущие размеры окна
	int _current_width, _current_height;
	
	int _maxActiveTextureId = 0; // Максимальный номер текстуры в glActiveTexture
	
	GLuint FBO;

	MatrixMode _currentMatrixMode;

	std::deque<math::Matrix4> _matrixStack[(size_t)MatrixMode::Count];
		// стэки сохраненных матриц для каждого типа матрицы
	
	math::Matrix4 _currentMatrix[(size_t)MatrixMode::Count];
		// текущие матрицы для каждого типа матрицы
	
	std::deque<IRect> _scissorsStack;
		// стек обрезаний для BeginClippoing и EndClipping

	// Объект, который управляет обрезанием вывода по полигону 
	ClipSpace _clipSpace;
	bool _clipSpaceValid;
	int _nextStencilValue;

	int _swapInterval;
		// Период обновления экрана
    
    MultisampleType _preferredMultisampleType;
        // тип мультисэплинга
    GLsizei _currentMultisamples;
        // текущее количество сэмплов в мультисэмплинге

	uint32_t _vramUsage;
	std::map<DEVCUBETEXTURE, uint32_t> _cubeTexVramUsage;
	std::map<DEVTEXTURE, uint32_t> _targetVramUsage;
};

extern RenderDeviceInterface& device;

ShaderProgramGLES2* loadShaderGLES2(const char* vertexFileName, const char* fragmentFileName);

} // namespace Render

#endif //_RENDERDEVICE_GL_ES2_H_

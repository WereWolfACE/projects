#ifndef _RENDER_RENDERDEVICE_INTERFACE_H_
#define _RENDER_RENDERDEVICE_INTERFACE_H_

#pragma once

#include "ClippingMode.h"
#include "Render/RestorableObject.h"
#include "Render/Texture.h"
#include "Render/Target.h"
#include "Render/RenderTypes.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBufferBuilder.h"
#include "SceneGraph/Material.h"
#include "platform.h"

namespace Core
{
	class Window;
	class Application;
}

namespace Render {

class Texture;
class Target;
class VolumeTexture;
class CubeTexture;
class ShaderProgram;

//
// Общий для всех платформ интерфейс рендердевайса
// При добавлении нового метода есть 2 варианта:
//  1) объявить его здесь как чисто виртуальный (= 0);
//  2) объявить его здесь и определить тривиальную версию в RenderDeviceInterface.cpp
//     (с обязательным использованием Assert(false)).
//
class RenderDeviceInterface {
public:
	RenderDeviceInterface();

	virtual ~RenderDeviceInterface() {}

	/// layer must be a CAEAGLLayer
	virtual void BindWindowGL(void* layer, int contextWidth, int contextHeight) { Halt(std::string("Override or don't call this method: ") + __FUNCTION__); }

	/// Задать viewport
	/// Координаты логические, внутри произойдет пересчет
	virtual void SetViewport(int x, int y, int width, int height) = 0;

	virtual IRect GetViewport() = 0;

	/// установить ортогональную матрицу проецирования
	/// ширина и высота логические, т.е. определяются игровыми координатами
	virtual void SetOrthoProjection(float logicalWidth, float logicalHeight, float minZ, float maxZ) = 0;

	virtual void Upload(Texture* tex) = 0;
	virtual void Upload(Texture* tex, Image& image) = 0;	
	virtual void Upload(const Image& image, DEVTEXTURE& _tex, bool bilinear = true, bool clamp = true, const std::string& texId = "") = 0;	
	virtual void UploadRegion(Texture* tex, int x, int y, int width, int height, const char* data) = 0;
	virtual void Reload(Texture *tex) = 0;
	virtual void Release(Texture* tex) = 0;
	virtual void Bind(Texture* tex, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT) = 0;

	virtual void Release(Target* tar) = 0;
	virtual void Bind(Target* rt, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT) = 0;

	virtual void Upload(VolumeTexture* tex) = 0;
	virtual void Release(VolumeTexture* tex) = 0;
	virtual void Bind(VolumeTexture* tex, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT) = 0;

	virtual void Upload(CubeTexture* tex) = 0;
	virtual void Release(CubeTexture* tex) = 0;
	virtual void Bind(CubeTexture* tex, int channel = 0, unsigned int stageOperation = STAGE_DEFAULT) = 0;

	virtual void Bind(ShaderProgram* shader) = 0;
	virtual void Release(ShaderProgram* shader) = 0;
	virtual ShaderProgram *GetBindedShader() = 0;
	virtual void ResetShader() = 0;

	virtual bool CanUseGPUSkinning(int bonesCount) = 0;

	virtual void SetViewFrustum(float left, float right, float bottom, float top, float zNear, float zFar) = 0;

	virtual void SetMatrixMode(MatrixMode mode) = 0;
	virtual MatrixMode GetMatrixMode() const = 0;

	virtual void PushMatrix() = 0;
	virtual void PopMatrix() = 0;

	virtual void ResetMatrix() = 0;

	virtual const math::Matrix4& GetMatrix(MatrixMode matrix) const = 0;
	virtual void SetMatrix(const math::Matrix4& matrix) = 0;
	virtual void MatrixMultiply(const math::Matrix4& matrix) = 0;
	virtual void MatrixTranslate(const math::Vector3& v) = 0;
	virtual void MatrixRotate(const math::Vector3& axis, float angle) = 0;
	virtual void MatrixScale(float scaleX, float scaleY, float scaleZ) = 0;

	void MatrixTranslate(float dx, float dy, float dz) { MatrixTranslate(math::Vector3(dx, dy, dz)); }
	void MatrixTranslate(const FPoint& offset) { MatrixTranslate(math::Vector3(offset.x, offset.y, 0.0f)); }
	void MatrixScale(float s) { MatrixScale(s, s, s); }

	/// Служебная функция для проверки глубины стека матриц
	virtual int GetStackSize(MatrixMode matrix) = 0;

	/// Включает или выключает текстурирование.
	/// При выключении обнуляются все использованные каналы текстур.
	/// При включении восстанавливается нулевой канал из последней текстуры.
	virtual void SetTexturing(bool enable) = 0;
	virtual bool GetTexturing() const = 0;

	virtual void SetCulling(bool cullEnable) = 0;

	virtual void SetBlendMode(BlendMode blendMode) = 0;
	virtual BlendMode GetBlendMode() const = 0;

	/// Прямое изменение текущего цвета.
	/// Рекомендуется использовать вместо этого метода метод Render::BeginColor.
	virtual void SetCurrentColor(const Color& color = Color::WHITE) = 0;
	virtual Color GetCurrentColor() const = 0;

	/// Устанавливает текущий материал для взаимодействия с освещением
	virtual void SetMaterial(SceneGraph::MaterialPtr material) = 0;

	// Вывод геометрии
	//

	/// Настраивает параметры отрисовки вершин для последующих множественных вызовов Draw
	virtual void SetupGeometry(const VertexElementDescription* decl, const void* addr = NULL) = 0;

	/// Настраивает шейдер для последующих множественных вызовов Draw
	virtual void SetupShader(const VertexElementDescription* decl, const void* addr = NULL) = 0;
	
	/// Рисует примитивы с помощью VBO
	virtual void DrawPrimitives(PrimitiveType type, const VertexBuffer& vertices, size_t elements_offset, size_t elements_count) = 0;
	virtual void DrawIndexedPrimitives(PrimitiveType type, const VertexBuffer& vertices, const IndexBuffer& indices, size_t elements_offset, size_t elements_count) = 0;
	
	/// Рисует примитивы из системной памяти
	virtual void DrawPrimitives(PrimitiveType type, const void *vertices, const VertexElementDescription* decl, size_t elements_offset, size_t elements_count) = 0;
	virtual void DrawIndexedPrimitives(PrimitiveType type, const void *vertices, const VertexElementDescription* decl, const uint16_t* indices, size_t elements_offset, size_t elements_count) = 0;

	template <typename VertexType>
	void DrawPrimitives(PrimitiveType type, const VertexType *vertices, size_t elements_offset, size_t elements_count) {
		DrawPrimitives(type, vertices, VertexType::GetDeclaration(), elements_offset, elements_count);
	}

	template <typename VertexType>
	void DrawIndexedPrimitives(PrimitiveType type, const VertexType *vertices, const uint16_t* indices, size_t elements_offset, size_t elements_count) {
		DrawIndexedPrimitives(type, vertices, VertexType::GetDeclaration(), indices, elements_offset, elements_count);
	}

	//
	//

	virtual void Begin2DMode() = 0;
	virtual void Begin2DModeInViewport(int x, int y, int width, int height) = 0;

	virtual void ResetViewport() = 0;

	virtual void End2DMode() = 0;

	// Внимание! При переопределении следует вызвать метод предка.
	virtual void BeginScene() = 0;

	// включает/выключает z-тест
	virtual void SetDepthTest(bool bEnable) = 0;

	// включает/выключает запись в буфер глубины
	virtual void SetDepthWrite(bool bEnable) = 0;

	/// Включает/отключает запись цвета и/или альфы во фреймбуффер
	virtual void SetColorWriteMask(bool red, bool green, bool blue, bool alpha) = 0;

	virtual void ClearDepthBuffer() = 0;

	// Внимание! При переопределении следует вызвать метод предка.
	virtual void EndScene() = 0;

	/// Очищает задний буфер
	virtual void Clear(const Color& color = Color::BLACK) = 0;

	virtual void Release() = 0;
	
	/// Вызываются все _d3dDevice->SetRenderState,
	/// одинаковые для BindWindow, Reset
	virtual void SetDefaultRenderState() = 0;

	/// Правда ли, что устройство вывода поддерживает текстуры только со
	/// сторонами, являющимися степенями двойки.
	virtual bool IsPower2Required() = 0;

	virtual void RegisterRestorableObject(RestorableObject* asset) = 0;
	virtual void UnregisterRestorableObject(RestorableObject* asset) = 0;

	/// Возвращает использование видео памяти в Мб
	virtual uint32_t GetVideoMemUsage() const = 0;

	/// Пересчитывает UV координаты для текущей текстуры
	/// Вот этот TranslateUV самый правильный :)
	virtual void TranslateUV(FRect& rect, FRect& uv) = 0;

	virtual void WriteVendorInfo() = 0;

	/// true если включена компенсация для полноэкранного режима на "высоком" экране
	virtual bool LetterBoxScreen() = 0;

	/// true если включена компенсация для полноэкранного режима на широком экране
	virtual bool PillarBoxScreen() = 0;

	/// размеры и положение изображения на экране (логические)
	virtual FRect ScreenPos() = 0;

	/// размеры и положение изображения на экране (физические)
	virtual FRect ScreenPosPhysical() = 0;

	virtual int Width() const = 0;
	virtual int Height() const = 0;
	virtual int PhysicalWidth() const = 0;
	virtual int PhysicalHeight() const = 0;
	
	/// Возвращает расстояние до плоскости отсечения.
	float getFrustumZNear() const { return _frustumZNear; } ///< Ближняя плоскость (обычно -1000.f)
	float getFrustumZFar() const { return _frustumZFar; } ///< Дальняя плоскость (обычно +1000.f)

	/// Возвращает указатель на текущую текстуру
	virtual Texture* GetBindedTexture() const = 0;

	virtual void SetBindedTexture(Texture* tex) = 0;

	virtual void ClearBindedObject() = 0;
	
	//
	// Начать режим отсечения прямоугольным окном window
	// Координаты окна в текущей системе координат,
	// которая может быть перемещена и повёрнута.
	// Перед отсечением разрешается вращение ТОЛЬКО вокруг оси Z
	//
	// Режим отсечения говорит, отсекать ли по каждой из четырёх сторон.
	// По умолчанию - отсекается всё. На количестве плоскостей отсечения
	// имеет смысл экономить, поскольку их может быть всего 6, т.е.
	// даже два прямоугольника отсечь проблематично.
	//
	virtual void BeginClipping(const IRect& window, ClippingMode clippingMode = ClippingMode::ALL) = 0;

	//
	// Завершение участка, открытого последним BeginClipping
	//
	virtual void EndClipping() = 0;

	
	//
	// Создает линию отсечения в текущей координатной системе.
	// Координаты линии задаются текущей системе координат,
	// которая может быть перемещена и повёрнута.
	// Перед отсечением разрешается вращение ТОЛЬКО вокруг оси Z.
	// Отсекает все, что находится справа от вектора (x1,y1)(x2,y2)
	// Предполагается, что вызовы данной функции не перемежаются с вызовами
	// BeginClipping / EndClipping, т.е. если последней вызывалась BeginClipping,
	// то первой должна вызваться EndClipping, и наоборот, для BeginClipPlane
	// должна вызываться EndClipPlane.	
	//
	virtual void BeginClipPlane(float x1, float y1, float x2, float y2) = 0;
	
	//
	// Выключает последнюю включенную линию отсечения
	//
	virtual void EndClipPlane() = 0;

	//
	// Создать цель рендеринга с нужными размерами.
	// Тот, кто создаёт таргет с помощью CreateRenderTarget _обязан_ 
	// вызвать DeleteRenderTarget, как только текстура становится не нужна.
	// При выходе из игры количество висящих в памяти объектов проверяется
	// на равернство с нулём. Таргеты можно удалять в деструкторах виджетов,
	// в этот момент Render::device еще живой.
	// Параметр alpha включает или отключает альфа-канал у цели
	// Если не требуется использовать альфу отрендеренного в неё изображения, то alpha стоит передать как false
	// Параметр multisampled отвечает за сглаживание при рендере в текстуру с таким же качеством, что и рендер в экранный буфер.
	// Таким образом, если сглаживание отключено при создании девайса, то и в целях сглаживания не будет.
	// Но если включено, то в цели будут multisampled, что делает невозможным сохранение их в image_pool, т.е.
	// их можно использовать только для рендера, а не для сохранения на диск
	// Параметр depth указывает на использование буфера глубины целью
	// Параметр msType позволяет задать качество мультисэмплинга таргета вручную. Если он равен MULTISAMPLE_NONE, при multisampled == true,
	// то цель будет использовать качество рендера экранного буфера.
	// Параметр stencil указывает на использование стенсил буфера в таргете
	//
	virtual Target* CreateRenderTarget(int width, int height, const RenderTargetConfig& config = RenderTargetConfig()) = 0;

	///
	/// Удалить рендертаргет и освободить память
	///
	virtual void DeleteRenderTarget(Target*& renderTarget) = 0;

	///
	/// Начать рендерить в рендер-таргет
	/// При этом начало координат будет в левом нижнем углу получающеся текстуры.
	/// Цвет фона будет (0, 0, 0, 0), то есть прозрачный.
	/// Если нужен непрозрачный фон, то можно отрендерить в таргет прямоугольник
	/// без текстуры обычным образом.
	/// Внимание! При переопределении следует вызвать метод предка.
	///
	virtual void BeginRenderTo(Target* render) = 0;

	void BeginRenderTo(Target* render, const Color& color) {
		BeginRenderTo(render);
		Clear(color);
	}

	///
	/// Завершить рендерить в рендер-таргет
	/// Внимание! При переопределении следует вызвать метод предка.
	///
	virtual void EndRenderTo() = 0;

	virtual void BeginRenderToTexture(Texture* texture) = 0;
	virtual void EndRenderToTexture() = 0;

	///
	/// Рекомендуется вызывать в конструкторе Application или его наследников.
	/// Установить _предпочтитаемый_ тип мультисэмплирования.
	/// Это не означает, что будет использоваться именно он;
	/// если он не поддерживается видеокартой, возьмётся ближайший меньший.
	///
	/// Мультисэмплирование задаёт режим полноэкранного антиалиасинга
	///
	virtual void SetPreferredMultisampleType(MultisampleType type) = 0;
	virtual MultisampleType GetPreferredMultisampleType() const = 0;

	/// 
	/// Включить/выключить вертикальную синхронизацию.
	/// Влияет только на полноэкранный режим, при включении немного страдает производительность.
	/// Имеет смысл вызывать в конструкторе Application (или  производного класса).
	///
	virtual void SetVSyncState(int intervals) = 0;

	///
	virtual int GetVSyncState() const = 0;

	/// Определение поддержки определённого вида шейдеров
	virtual bool IsVertexShaderSupported(int majorVersion, int minorVersion) = 0;

	/// Определение поддержки определённого вида шейдеров
	virtual bool IsPixelShaderSupported(int majorVersion, int minorVersion) = 0;

	/// Определение поддержки кубических текстур
	virtual bool IsCubeTextureSupported() = 0;
	
	/// Определение поддержки объёмных текстур
	virtual bool IsVolumeTextureSupported() = 0;

	/// Максимальное количество текстур, поддерживаемых при мультитекстурировании.
	/// В общем случае верно для фиксированного конвейера, т.к. количество текстур
	/// поддерживаемых в шейдерах зависит от версии шейдеров и может отличаться.
	virtual int GetMaxTextureUnits() = 0;

	/// Создать контекст OpenGL
	/// Должна вызываться в начале каждого потока (и главного тоже)
	/// Также _обязательно_ нужно вызывать парный метод DestroyContext 
	virtual void CreateGLContext() = 0;
	
	///
	/// Удалить контекст OpenGL
	/// Нужно вызывать парно CreateGLContext в конце каждого потока
	///
	virtual void DestroyGLContext() = 0;
	
	// Завершить все команды OpenGL (актуально для iOS, где
	// используется отложенное выполнение команд)
	virtual void GLFinish() const = 0;
	
	virtual void SetPresentParams(int w = 800, int h = 600) = 0;
	
	// задать размеры рендер-буфера при изменении размеров области экрана, в которой рисуется приложение
	virtual void SetRenderBufferSize(int width, int height) = 0;

	virtual void SetScissorTest(bool enable) = 0;

	// Функции для работы с буфером трафарета
	virtual void SetStencilTest(bool enable) = 0;
	virtual void SetStencilFunc(StencilFunc func, int value, uint32_t mask = ~0) = 0;
	virtual void SetStencilOp(StencilOp fail, StencilOp zFail, StencilOp pass) = 0;
	
	// Происходит ли рендер в таргет
	bool IsRenderingToTarget() const { return _isRenderingToTarget; }	
	bool IsRenderingToScreen() const { return _isRenderingToScreen; }

protected:
	/// Расстояние до ближней и дальней плоскостей отсечения по умолчанию.
	/// Используется, например, в Begin2DMode().
	const float _defaultZNear;
	const float _defaultZFar;

	/// Расстояние до плоскостей отсечения на данный момент.
	/// Может отличаться от _defaultZNear/Far после вызова
	/// SetOrthoProjection(..) или SetViewFrustum(..).
	float _frustumZNear;
	float _frustumZFar;

	typedef std::set<Texture*> TextureSet;
	// list of uploaded textures, for dx - dynamic only, for opengl - all
    TextureSet _dynamicTextures;

	typedef std::set<Target*> TargetSet;
	// list of uploaded render targets
	TargetSet _renderTargets;

	typedef std::set<RestorableObject*> RestorableObjectSet;
	// list of restorable device assets
	RestorableObjectSet _restorableObjects;

private:
	bool _isRenderingToTarget;
		// рендерим ли в цель (находимся между BeginRenderTo и EndRenderTo)

	bool _isRenderingToScreen;
		// рендерим ли на экран (находимся между BeginScene и EndScene)
};

extern RenderDeviceInterface& device;
	
} // namespace Render

#endif // _RENDER_RENDERDEVICE_INTERFACE_H_

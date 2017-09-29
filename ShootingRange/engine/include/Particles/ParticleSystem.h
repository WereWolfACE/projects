#ifndef PARTICLE_SYSTEM_VER2_H
#define PARTICLE_SYSTEM_VER2_H

#include "Particles/EmitterMask.h"
#include "Particles/TimeParam.h"
#include "Render/Sheet.h"

namespace Render {
	class Texture;
	class SpriteBatch;
}

class Particle;

// типы эмиттера, значения прописаны явно, т.к. используются в сериализации и не должны изменяться
enum class EmitterType : uint8_t {
	Point = 0,
	Line = 1,
	Area = 2,
	Circle = 3,
	Mask = 4
};

/// Система частиц.
/// Представляет собой управляемое множество одинаковых частиц.
class ParticleSystem : public RefCounter {
public:
	static const uint32_t serializerFourcc = MAKEFOURCC('P', 'S', 'y', 's');
public:
	template<class Serializer>
	void Serialize(Serializer &serializer, int version);

	void Reset();
	void Update(float dt);
	void Draw(Render::SpriteBatch* batch = NULL);
	void DrawBlend(Render::SpriteBatch* batch = NULL);

	bool HitTest(int x, int y) const;

	bool IsAdditive() const { return _parameters->_blend == BlendMode::Add; }
	bool IsDead() const { return _isDead; }
	bool IsPermanent() const { return !_parameters->_needStartDeadCounter; }

	void Finish();
	void SetScale(float scale_x, float scale_y = 0);
	void SetPositionOffset(float x, float y);
	void SetAlphaFactor(float alpha);
	void SetRGBFactor(float r, float g, float b);

	int FrameWidth() const { return _parameters->_frameWidth; }
	int FrameHeight() const { return _parameters->_frameHeight; }
	
	float GetLocalTime() const { return _localTime; }
	void SetLocalTime(float localTime) { _localTime = localTime; }

	const std::string& GetName() const { return _parameters->_name; }
	
	const std::string& GetTextureName() const { return _parameters->_texName; }
	Render::Texture* GetTexture() const { return _parameters->GetTexture(); }

	EmitterMask* GetEmitterMask() const { return _parameters->GetEmitterMask(); }

	size_t MemoryInUse() const;

private:
	//параметры системы частиц
	class Parameters : public RefCounter
	{
	public:
		Parameters();
		~Parameters();
		Parameters(const Parameters& value);
		ParticleSystem& operator=(const ParticleSystem&) = delete;

		template<class Serializer>
		void Serialize(Serializer &serializer, int version, ParticleSystem &owner);

		bool IsVelocity() const { return isVelocity; }
		bool IsAnimation() const { return _isAnimation; }
		Render::Texture* GetTexture() const { return _tex.get(); }
		Render::Texture* GetEmitterTexture() const { return emitterTex.get(); }
		EmitterMask* GetEmitterMask() const { return emitterMask.get(); }

		// частицы перерождаются после паузы
		inline bool HasPauseBeforeReborn() const;
		// время паузы перед перерождением
		inline float GeneratePauseBeforeRebornTime() const;
		// время жизни частицы
		inline float GenerateLifeTime() const;

		uint16_t GetLastAnimationFrame() const;

		FPoint GenerateParticle(int index, float &spinStart) const;

		size_t MemoryInUse() const;

		bool IsLocalTextures() const { return _localTextures; }
		void UseLocalTextures(bool localTextures);

		// загружает текстуру частицы, создаёт маску эмиттера и загружает его текстуру
		// вызывается один раз после загрузки или создания системы частиц
		void LoadTextures(
			bool localTextures,
			const std::string &effectName,
			const std::vector<Render::SheetPtr> *atlases,
			const EmitterMaskParameters *maskParameters,
			const EmitterMaskFileReferences &masks,
			std::stringstream &warnings,
			std::stringstream &errors);

	private:
		// для редактора эффектов
		friend class EffectDocument;

		// загружает текстуру частицы
		void LoadTexture(const std::vector<Render::SheetPtr> *atlases, std::stringstream &errors);
		// загружает текстуру маски эмиттера
		void LoadEmitterTexture(std::stringstream &errors);
		void CreateTextureMask(
			const std::string &effectName,
			const EmitterMaskParameters &parameters,
			const EmitterMaskFileReferences &masks,
			std::stringstream &warnings);
		void DestroyTextureMask();
		void ClearTextures();

		static void LoadLocalTexture(const std::string& filename, Render::TexturePtr &textureStore, std::stringstream &errors);
		static void UseLocalTexture(Render::TexturePtr textureStore);
		static void ReleaseLocalTexture(Render::TexturePtr &textureStore);

	public:

		// система видима (используется только в редакторе, не сохраняется в файлы эффектов)
		bool _isVisible = true;
		// система спрятана (невидима), в отличии от _isVisible сохраняется в файлы эффектов
		bool _isHidden = false;
		// true - используется скорость и направление (v, spin), false - используется положение (x,y)
		bool isVelocity = false;
		// анимация текстуры
		bool _isAnimation = false;
		// частицы смещаются вместе с эффектом
		bool linkParticles = false;
		// частицы поворачиваются вдоль направления ориентации
		bool orientParticles = false;
		// ориентация частиц равномерно распределена по всем направлениям
		bool fixedAngleStep = false;
		// прямоугольные частицы
		bool _isScaledNonproportional = false;
		// частицы создаются одновременно
		bool _isEqualCreateTime = false;
		// система не циклическая
		bool _needStartDeadCounter = false;
		// эмиттер - сплошная фигура (прямоугольник, эллипс), false - рамка по границе
		bool _emitterSolid = false;
		// показать текстуру маски эмиттера (испольуется только в редакторе, сохраняется в файлы эффектов)
		bool showEmitter = false;

		// число частиц
		short _numOfParticles = 1;

		// режим смешивания
		BlendMode _blend = BlendMode::Alpha;

		// параметры эмиттера

		// тип эмиттера
		EmitterType emitType = EmitterType::Point;

		// ориентация эмиттера
		float emitterOrientation = 0;
		// центральное направление испускания частиц из эмиттера
		float emitterAngle = 0;
		// разброс направлений испускания частиц из эмиттера
		float emitterRange = 0;
		// размеры эмиттера
		float emitterSizeX = 1.0f;
		float emitterSizeY = 1.0f;
		// толщина рамки эмиттера (прямоугольник, эллипс)
		float emitterFactor = 0;
		// смещение эмиттера
		float _emitterOffsetX = 0;
		float _emitterOffsetY = 0;

		// время жизни частиц
		float _lifeInitial = 1.0f;
		float _lifeVariation = 0;

		// пауза после времени жизни частиц
		float _pauseAverage = 0;
		float _pauseVariation = 0;

		// время в течении которого создаются все частицы
		float _startTime = 0;

		// время начала жизни системы
		float bornTime = 0;

		// время, когда новые частицы перестают создаваться
		float _deadCountTime = 0;

		// имя системы частиц
		std::string _name;

		// имя текстуры частиц
		std::string _texName;

		// размеры спрайтов анимации в текстуре частиц
		short _frameWidth = 1;
		short _frameHeight = 1;

		// смещение центра частицы от центра текстуры
		FPoint _hotPoint = FPoint(0.5f, 0.5f);

		// сплайновые параметры
		TimeParam x, y, size, angle, v, spin, colorRed, colorGreen, colorBlue, colorAlpha, fps, ySize;

	private:
		// система цастиц созданная с этим флагом читает текстуру частиц, текстуру эмиттера и данные EmitterMask 
		// не через менеджер ресурсов, а напрямую из файлов текстур
		bool _localTextures = false;

		// ресурс маски эмиттера
		EmitterMaskPtr emitterMask;
		// текстура маски эмиттера
		Render::TexturePtr emitterTex;
		// текстура частиц
		Render::TexturePtr _tex;
	};
	typedef boost::intrusive_ptr<Parameters> ParametersPtr;

private:
	friend class ParticleEffect;
	friend class ParticleEffectLoader;
	friend class Particle;
	// для редактора эффектов
	friend class EffectDocument;
	friend class SerializationTest;

	ParticleSystem();
	ParticleSystem(const ParticleSystem&);
	~ParticleSystem();
	ParticleSystem& operator=(const ParticleSystem&) = delete;

	// полный клон, с собственными параметрами
	ParticleSystem *FullClone() const;

	// частичный клон, параметры общие
	ParticleSystem *PartialClone() const;

	void RebornParticle(Particle& part, int index);

	void InternalDraw(Render::SpriteBatch* batch, float alpha);

	// обработка окончания времени жизни частицы и её пересоздание
	// возвращает true если нужно пересоздать частицу
	inline bool ProcessParticleRebornTime(Particle &particle) const;
	// координаты центра частицы
	inline FPoint GetParticleCenter(const Particle &particle) const;

	// загружает текстуру частицы, создаёт маску эмиттера и загружает его текстуру
	// вызывается один раз после загрузки или создания системы частиц
	void LoadTextures(
		bool localTextures,
		const std::string &effectName,
		const std::vector<Render::SheetPtr> *atlases,
		const EmitterMaskFileReferences &masks,
		std::stringstream &warnings,
		std::stringstream &errors);

private:
	// номер системы в эффекте, используется как временное хранилище во время расчётов, в остальное время значение должно быть -1
	size_t _effectIndex = -1;

	typedef std::vector<Particle> Particles;
	Particles _particles;

	size_t _deadParticleCounter = 0;

	float _localTime = 0;

	// объект - владелец параметров, менять параметры может только владелец
	bool _parametersOwner = true;
	// ресурсы системы (текстуры, маска) заблокированы от выгрузки
	bool _resourcesLocked = false;
	bool _needParticleReborn = true;
	bool _needDeadCount = false;
	bool _allParticlesIsDead = false;
	bool _isFreeze = true;
	bool _isDead = false;

	// масштаб
	float _scale_x = 1.0f, _scale_y = 1.0f;

	// окраска
	float redFactor = 1.0f, greenFactor = 1.0f, blueFactor = 1.0f, alphaFactor = 1.0f;

	// положение
	float posX = 0, posY = 0;

	// временное хранилище параметров маски эмиттеры, используется только при чтении эффектов, в остальное время пустое
	// также используется в редакторе для хранения имени файла маски
	EmitterMaskParametersPtr _emitterMaskParameters;

	// параметры системы частиц, разделяюца между несколькими системами
	ParametersPtr _parameters;

	// привязка эмиттера к другой системе частиц
	intrusive_weak_ptr<ParticleSystem> _emitterLink;

public:
	static std::string NormalizeTextureName(const std::string &name);
	static void SetTexturesPath(const std::string &path);
	static const std::string& GetTexturesPath();

private:
	static Render::TexturePtr _texture_plug;
	static std::string _textures_path;

private:
	static void CreateTexturePlug();
	static void DestroyTexturePlug();

	void LockResources();
	void UnlockResources();

	void CreateOwnParameters();
	const Parameters *GetParameters() const { return _parameters.get(); }
	Parameters *GetOwnerParameters() { Assert(_parametersOwner); return _parameters.get(); }
};

typedef boost::intrusive_ptr<ParticleSystem> ParticleSystemPtr;

#endif
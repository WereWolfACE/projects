#ifndef _PLAYRIX_LAYER_H_
#define _PLAYRIX_LAYER_H_

#include "GUI/Widget.h"
#include "MessageFunc.h"
#include "RefCounter.h"

#include <boost/signals2.hpp>

namespace Render {
	class Texture;
}

struct LayerFlags {
	enum Type {
		ENABLE_ALL = 0,
		DISABLE_DRAW = 1,
		DISABLE_UPDATE = 2,
		DISABLE_ALL = DISABLE_DRAW | DISABLE_UPDATE
	};
};

enum class LayerState {
	Hidden,		// нет на экране
	Visible,	// есть на экране
	Init,		// в процессе вывода на экран
	Deinit		// в процессе удаления с экрана
};

/// Обработчик событий слоя
class LayerHandler {
public:
	virtual ~LayerHandler();

	virtual void OnLayerMessage(const Message& message) = 0;
		// "Init"
		// "Deinit"

	void ConnectToLayer(Layer* layer);
	void DisconnectLayer(Layer* layer);
	void DisconnectAll();

private:
	typedef std::map<Layer*, boost::signals2::connection> Connections;
	Connections _connections;
};

class Layer : public RefCounter
{
public:
	Layer();
	Layer(rapidxml::xml_node<>* elem);
	virtual ~Layer();
	
	int GetFlags() const { return _flags; }
	void SetFlags(int flags) { _flags = flags; }
	
	GUI::Widget* GetActiveWidget() const;
	void SetActiveWidget(GUI::Widget* w);
	void ResetWidgetPositions();
	void Load(rapidxml::xml_node<>* elem);
	void Draw(float scale=1.f, float angle=.0f);
	void AcceptMessage(const Message& message);
	void BroadcastMessage(const Message& message);
	void AddWidget(GUI::Widget* widget);
	void UpdateRotateCenter(const std::string& widgetName);
	void DeleteWidget(const std::string& name);
	GUI::Widget* getWidget(const std::string& name);
	bool MouseDown(const IPoint &mousePos);
	void MouseUp(const IPoint &mousePos);
	void MouseMove(const IPoint &mousePos);
	void MouseCancel();
	void Update(float dt);
	void KeyPressed(int keyCode);
	void KeyReleased(int keyCode);
	void CharPressed(int unicodeChar);
	void KeyFocusCancel();
	void MouseWheel(int delta);

	bool PinchBegan(float scale, const IPoint &position);
	void PinchChanged(float scale, const IPoint &position);
	void PinchEnded();
	
	float GetScale() const { return scale; };

	// Существует ли виджет с таким именем
	bool WidgetExists(const std::string& name) const;

	// Вернуть первый виджет в лэйере
	GUI::Widget* GetFirstWidget() const;
	
	// Грузит ресурсы и вызывает lua инициализатор
	void InitLayer();
	
	// Выгружает ресурсы и вызывает lua деинициализатор
	void DeinitLayer();
	
	void UploadResources();
	
	void ReleaseResources();
	
	// Прозрачен ли для событий ввода
	bool IsInputTransparent() const;

	// Установить прозрачность для событий ввода
	void SetInputTransparent(bool value);

	// Устанавливает флаг, указывающий, можно ли производить 
	// деинициализацию слоя (выгрузку ресурсов группы слоя)
	void SetAllowDeinit(bool allow);

	// Можно ли деинициализировать слой (выгрузить его ресурсы)
	bool IsAllowDeinit() const;

	// Возвращает виджеты слоя
	const std::vector<GUI::WidgetPtr>& GetWidgets() const;

	LayerState GetState() const;
private:
	void LoadWidgets(rapidxml::xml_node<>* elem, GUI::Widget *parent = NULL);
	
	// Пересчёт позиции мыша с учётом поворота, смещения и масштаба
	IPoint CalcMousePos(const IPoint& mousePos);
	
	void DeleteWidget(GUI::Widget* w);

	// Подсоединяет обработчик событий слоя.
	// Временем жизни обработчика управляет вызывающая сторона.
	boost::signals2::connection ConnectHandler(LayerHandler* handler);

	friend void LayerHandler::ConnectToLayer(Layer*);

public:
	std::string name;

	MessageFuncPtr messageFunc;

	math::Vector3 position;
	math::Vector3 centerRotation;

	float angleZ;
	float scale;
	
	Color color;
	
	bool allowDeinit; // Позволить ли деинициализацию слоя

	bool inputDisabled; // Запретить обработку событий ввода.
		// Удобно устанавливать на время появления/исчезновения слоя.

private:
	GUI::Widget *_activeWidget;
	
	Render::Texture* _background;
	
	typedef std::map<std::string, GUI::Widget*> WidgetsMap;
	WidgetsMap _widgetsMap;
	
	typedef std::vector<GUI::WidgetPtr> Widgets;
	Widgets _widgets;
	
	typedef std::set<std::string> ResourceGroups;
	ResourceGroups _resourceGroups;
		// Группы ресурсов привязанные к слою. Они будут загружаться и выгружаться автоматически.
	
	bool _isInputTransparent;
		// Прозрачен слой ли для событий ввода (мышь, клавиатура);
		// прозрачные для событий слои получают их, но не задерживают,
		// таким образом, события доходят до нижних слоев;
		// флаг регулируется в XML атрибутом inputTransparent с возможными значениями "true" или "false";
		// значение по умолчанию - "false".

	int _flags; // Флаги LayerFlags

	typedef boost::signals2::signal<void(const Message&)> Signal;
	Signal _signal;

	LayerState _state;
};

typedef boost::intrusive_ptr<Layer> LayerPtr;

#endif

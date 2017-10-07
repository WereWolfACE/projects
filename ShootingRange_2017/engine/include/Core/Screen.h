#ifndef _CORE_SCREEN_H_
#define _CORE_SCREEN_H_

#include <boost/function.hpp>

class Layer;
class Message;

namespace Core
{

///
/// Экран
///
class ScreenImpl 
{
public:
	ScreenImpl();
	
	void ProcessMessage(const Message& message); /// Обработать сообщение
	void ProcessMessages(); /// Обработать отложенные сообщения (вызывается из application)
	
	///
	/// Функция рисования
	///
	void Draw();
	
	/// Отрисовать только нижние слои
	void DrawLowerLayers();
	/// Отрисовать только верхние слои (те которые в эффектах, они рисуются без альфы и прочих бантиков)
	void DrawUpperLayers();
	
	///
	/// Вернуть следующий за верхним слой (проверка существует-ли он на совести вызывающего)
	///
	Layer* GetSecondLayer() const;
	
	///
	/// Вернуть верхний слой (проверка существует-ли он на совести вызывающего)
	///
	Layer* GetTopLayer() const;
	Layer* GetTopMouseOpaqueLayer() const;

    ///
	/// Вернуть слой следующий за текущим (проверка существует-ли он на совести вызывающего)
	///
	Layer* GetLayerNextTo(const std::string& layerName) const;

    ///
	/// Вернуть имя слоя следующего за текущим (проверка существует-ли он на совести вызывающего)
	///    
    std::string GetLayerNameNextTo(const std::string& layerName) const;
    
	///
	/// Очистить экран
	///
	void Clear();
	
	///
	/// Затолкать слой в стек
	///
	void pushLayer(Layer* layer);
	void pushLayer(const std::string& layerName);

	void pushLayerBefore(const std::string& layerName, const std::string& beforeLayerName);
	void pushLayerAfter(const std::string& layerName, const std::string& afterLayerName);


	/// Удалять слои с низа стека до тех пор, пока не встретится указанный слой
	void popBackgroundUntilLayer(Layer* layer);
	void popBackgroundUntilLayer(const std::string& layerName);
	
	/// Асинхронно загружает группы ресурсов слоя, после чего вызывает событие
	void loadGroups(Layer* layer, const boost::function<void(Layer*)>& callback);
	
	/// Установить данный слой для отладочной информации. Этот слой будет получать копии всех сообщений.
	/// Допускается передавать null.
	void setDebugLayer(Layer* layer);
	void setDebugLayer(const std::string& layerName);
	///
	/// Вернуть имя верхнего лейера
	/// (не рекомендуется использовать,
	/// в большинствве случаев лучше вызывать GetTopLayer())
	///
	std::string getMainLayerName() const;
	
	///
	/// Выплюнуть слой
	///
	void popLayer();
	
	///
	/// Реакция на течение времени
	///
	void Update(float dt);

	///
	/// Удаляет первый (ближайший к игроку) лейер с таким именем
	///
	void RemoveLayer(const std::string& layerName);
	
	bool isLayerOnScreen(const std::string &layerName);
	
	/// Доступен ли слой для событий, то есть все слои над ним - transparent.
	bool isLayerAccessible(Layer* layer) const;
	bool isLayerAccessible(const std::string &layerName) const;
	
	/// Обработка мышы и клавиатуры
	void MouseDown(const IPoint& mousePos);
	/// Обработка мышы и клавиатуры
	void MouseUp(const IPoint& mousePos);
	/// Обработка мышы и клавиатуры
	void MouseMove(const IPoint& mousePos);
	/// Обработка мышы и клавиатуры
	void MouseWheel(int delta);
	/// Обработка мышы и клавиатуры
	void MouseCancel();
	/// Обработка мышы и клавиатуры
	void KeyPressed(int keyCode);
	/// Обработка мышы и клавиатуры
	void KeyReleased(int keyCode);
	/// Обработка мышы и клавиатуры
	void CharPressed(int unicodeChar);
	/// Потеря фокуса ввода (не посылаются сообщения о нажатии и отпускании клавиш)
	void KeyFocusCancel();
    
	void PinchBegan(float scale, const IPoint &position);
	void PinchChanged(float scale, const IPoint &position);
	void PinchEnded();
    
	/// Экран не реагирует на ввод пользователя
	bool isLocked() const;
	/// Ручная блокировка/разблокировка на ввод
	void Lock();
	void Unlock();

	// Возвращает информацию о текущих слоях (для отладки)
	std::string GetDebugInfo();
	
	float GetInactivityTime() { return _inactivity_t; }
	void ResetInactivityTime(float t = 0.0f) { _inactivity_t = t; }
    
    int getLayersCount() const;
	Layer* getLayer(int index) const;

private:
	typedef std::list<Layer*> Layers;

	mutable MUTEX_TYPE _mutex;
	
	/// список слоёв
	Layers _layers;
	
	Layer* _debugLayer;
	
	float _inactivity_t; ///< Таймер для подсчёта времени, когда игрок ничего не делает

	int _lockCounter;

private:
	static void InternalDraw(const Layers& layers, float scale = 1.0f, float angle = 0.0f);
	static std::string GetDebugLine(const Layers& layers);
};

extern ScreenImpl mainScreen;

} // namespace Core

#endif

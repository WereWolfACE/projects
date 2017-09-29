#ifndef RESTORABLE_OBJECT_H
#define RESTORABLE_OBJECT_H


class RestorableObject
{
public:
	RestorableObject()
		: _isLost(false)
	{}

	virtual ~RestorableObject() {}

	// Вызывается при потере контекста. Утерянный ресурс нужно сбросить (занулить) и привести объект к некоторому целостному состоянию.
	// Также нужно установить флаг _isLost, если ресурс действительно был утерян.
	virtual void OnContextLost() = 0;

	// Вызывается после сброса всех ресурсов.
	// Если ресурс можно легко и быстро восстановить после потери контекста (загрузить из файла или рассчитать), то можно сделать это здесь.
	// Может ничего не делать, если простое восстановление невозможно.
	virtual void TryRestore() {}

	// Проверка на потерю ресурса, если вернет true, то ресурс нужно пересоздать в клиентском коде,
	// например, в методе EngineAppDelegate::OnContextRecreated.
	bool IsLost() const {
		return _isLost;
	}

protected:

	bool _isLost;

	// Нужно вызвать при создании ресурса, чтобы реагировать на потерю контекста
	void RegisterForContextLoss();

	// Нужно вызвать при уничтожении ресурса
	void UnregisterFromContextLoss();

	// Нужно вызвать при пересоздании gl-объекта.
	void FinishRestore() {
		_isLost = false;
	}

};

#endif // RESTORABLE_OBJECT_H

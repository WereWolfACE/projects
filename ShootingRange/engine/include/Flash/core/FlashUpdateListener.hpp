#pragma once

#include <deque>

#include "IFlashDisplayObject.h"
#include "FlashMarkSweep.h"

/**
* Используется для того, чтобы Update на объектах вызывался после обхода всей иерархии, а не во время.
* Таким образом решается проблема модификации дерева объектов из Update
*/
class FlashUpdateListener
{
public:
	/**
	* Указывает, что объект displayObject надо обновить на dt секунд
	*/
	void addUpdateRequest(IFlashDisplayObject* displayObject, float dt)
	{
		if ( displayObject->hasUpdate() )
		{
			UpdateRequest _request(displayObject, dt);
			GC_ADD_REF(displayObject);

			sheduledUpdates.push_back(_request);
		};
	};

	/**
	* Выполняет все запланированные обновления
	*/
	void doUpdates()
	{
		for (auto l_it = sheduledUpdates.begin(); l_it != sheduledUpdates.end(); ++l_it)
		{
			l_it->target->update(l_it->dt);
		}

		clear();
	}
		
	/**
	* Очищает очередь обновления
	*/
	void clear()
	{
		for (auto l_it = sheduledUpdates.begin(); l_it != sheduledUpdates.end(); ++l_it)
		{
			GC_REMOVE_REF(l_it->target);
		};

		sheduledUpdates.clear();
	}

	struct UpdateRequest
	{
		UpdateRequest(IFlashDisplayObject* _target, float _dt):
			target(_target),
			dt(_dt)
		{
		};

		IFlashDisplayObject* target;
		float dt;
	};

	std::deque<UpdateRequest> sheduledUpdates;
};

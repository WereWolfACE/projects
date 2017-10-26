#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

///
/// Асинхронная рабочая очередь
///
/// При вызове \ExecuteAsync(asyncFunc, completionHandler)
/// \asyncFunc будет выполнен в фоновом потоке, а
/// \completionHandler - в контексте вызова метода Update()
///
class AsyncWorkingQueue
{
public:
	enum { MINIMUM_THREAD_COUNT = 2 };
	enum QueueType { DEFAULT_QUEUE, POST_RESOURCE_LOAD_QUEUE, QUEUE_COUNT };

	typedef std::function<void()> Handler;
	typedef std::function<void(bool, const char*)> CompletionHandler; ///< Handler(is_error, message)
	typedef size_t ItemID;

	AsyncWorkingQueue();

	/// Запускает фоновые потоки
	void Start(size_t threadPoolSize = 0u);
	void Stop();
	void SetPaused(bool paused);
	bool IsPaused() const;

	/// Возвращает размер очереди активных обработчиков
	size_t GetHandlersCount(QueueType queue = DEFAULT_QUEUE) const;

	/// Выполняет код в фоновом потоке, после чего вызывает обработчик завершения
	ItemID Execute(Handler asyncFunction, CompletionHandler completionHandler, QueueType queue = DEFAULT_QUEUE);

	/// Удаляет обработчик из очереди
	void Remove(ItemID queueItemId);

	/// Синхронно выполняет завершившиеся обработчики количеством не более указанного
	/// \return количество выполненных обработчиков
	size_t ExecuteReady(size_t handlersToExecute);
	size_t ExecuteReady(size_t handlersToExecute, QueueType queue);

	/// Синхронно выполняет указанный обработчик и удаляет его из очереди
	void ExecuteNow(ItemID queueItemId);

	/// Синхронно выполняет все обработчики в очереди
	void ExecuteAllNow(QueueType queue = DEFAULT_QUEUE);

private:
	typedef boost::shared_future<void> Future;
	typedef std::pair<Future, CompletionHandler> AsyncHandler;
	typedef std::pair<ItemID, AsyncHandler> AsyncHandlerItem;
	typedef std::list<AsyncHandlerItem> AsyncHandlers;

	/// Асинхронная очередь
	class Queue {
	public:
		boost::asio::io_service io;
		boost::asio::io_service::work work;
		boost::thread_group threads;

		AsyncHandlers handlers; ///< Коллекция обработчиков для фонового выполнения

		Queue();
		~Queue();

		void Start(size_t threadCount);
		void Interrupt();
		void Stop();
	};

	Queue _queues[QUEUE_COUNT];

	ItemID _lastItemId; ///< Последний сгенерированный id для элемента

	boost::mutex _mutex;

	mutable boost::mutex _paused_mutex;
	boost::condition_variable _pause_changed;
	bool _paused;

private:
	/// Вспомогательный метод для выполнения обработчиков
	bool ExecuteHelper(AsyncHandler& item, bool wait);
	
	void BlockWhilePaused();

	// Копирование и присваивание запрещено
	AsyncWorkingQueue(const AsyncWorkingQueue&);
	AsyncWorkingQueue& operator=(const AsyncWorkingQueue&);
};

namespace Core {
	extern AsyncWorkingQueue asyncWorkingQueue;
}

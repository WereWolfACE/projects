#pragma once

class HttpRequest;

enum class InternetState {
	Unknown,
	Offline,
	Online
};

///
/// Класс для проверки доступности Интернета.
///
class InternetReachability {
public:
	enum {
		/* milliseconds */
		kInitTimeout = 5 * 1000,
		kMaxTimeout = 600 * 1000
	};

	typedef boost::function<void(const InternetReachability*)> Callback;

	explicit InternetReachability(boost::asio::io_service& io_service);
	~InternetReachability();

	/// Устанавливает таймаут повторной успешной проверки
	void SetSuccessTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetSuccessTimeout() const;

	/// Устанавливает начальный таймаут повторной неудачной проверки.
	/// При последующих неудачах таймаут удваивается, но не превышает максимального значения 10 мин.
	void SetFailureTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetFailureTimeout() const;

	/// Запускает процесс проверки.
	/// Выполняющийся процесс при этом будет остановлен.
	void Start(const HttpRequest& request);
	void Start(const std::string& url);

	/// Возвращает true, если в данный момент выполняется проверка
	bool IsRunnung() const;

	/// Останавливает процесс проверки
	void Cancel();

	/// Устанавливает функцию обратного вызова на обновление состояния подключения
	void OnStateUpdated(Callback callback);

	const HttpRequest& GetRequest() const;

	/// Текущее состоние подключения к Интернету
	void SetInternetState(InternetState state);
	InternetState GetInternetState() const;

	/// Время на сервере во время последнего подключения к нему
	void SetLastServerTime(time_t time);
	time_t GetLastServerTime() const;

private:
	void OnConnectionSuccess();
	void OnConnectionFailure(const boost::system::error_code& ec);

	void CancelConnection();

private:
	/// Временной интервал между проверками
	boost::posix_time::time_duration _success_timeout = boost::posix_time::milliseconds(kInitTimeout);
	boost::posix_time::time_duration _failure_timeout = boost::posix_time::milliseconds(kInitTimeout);

	/// Количество попыток подключения до первой успешной
	size_t _trials = 0;

	/// Время на сервере во время последнего подключения
	time_t _last_server_time = 0;

	/// Состояние подключения к Интернету
	InternetState _state = InternetState::Unknown;

	/// Этот объект вызывается при смене состояния
	Callback _callback;

	class Impl;
	std::unique_ptr<Impl> _impl;

	boost::asio::deadline_timer _repeat_timer;
};

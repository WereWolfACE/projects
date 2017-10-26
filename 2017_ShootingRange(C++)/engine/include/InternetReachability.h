#pragma once

class HttpRequest;

enum class InternetState {
	Unknown,
	Offline,
	Online
};

///
/// ����� ��� �������� ����������� ���������.
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

	/// ������������� ������� ��������� �������� ��������
	void SetSuccessTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetSuccessTimeout() const;

	/// ������������� ��������� ������� ��������� ��������� ��������.
	/// ��� ����������� �������� ������� �����������, �� �� ��������� ������������� �������� 10 ���.
	void SetFailureTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetFailureTimeout() const;

	/// ��������� ������� ��������.
	/// ������������� ������� ��� ���� ����� ����������.
	void Start(const HttpRequest& request);
	void Start(const std::string& url);

	/// ���������� true, ���� � ������ ������ ����������� ��������
	bool IsRunnung() const;

	/// ������������� ������� ��������
	void Cancel();

	/// ������������� ������� ��������� ������ �� ���������� ��������� �����������
	void OnStateUpdated(Callback callback);

	const HttpRequest& GetRequest() const;

	/// ������� �������� ����������� � ���������
	void SetInternetState(InternetState state);
	InternetState GetInternetState() const;

	/// ����� �� ������� �� ����� ���������� ����������� � ����
	void SetLastServerTime(time_t time);
	time_t GetLastServerTime() const;

private:
	void OnConnectionSuccess();
	void OnConnectionFailure(const boost::system::error_code& ec);

	void CancelConnection();

private:
	/// ��������� �������� ����� ����������
	boost::posix_time::time_duration _success_timeout = boost::posix_time::milliseconds(kInitTimeout);
	boost::posix_time::time_duration _failure_timeout = boost::posix_time::milliseconds(kInitTimeout);

	/// ���������� ������� ����������� �� ������ ��������
	size_t _trials = 0;

	/// ����� �� ������� �� ����� ���������� �����������
	time_t _last_server_time = 0;

	/// ��������� ����������� � ���������
	InternetState _state = InternetState::Unknown;

	/// ���� ������ ���������� ��� ����� ���������
	Callback _callback;

	class Impl;
	std::unique_ptr<Impl> _impl;

	boost::asio::deadline_timer _repeat_timer;
};

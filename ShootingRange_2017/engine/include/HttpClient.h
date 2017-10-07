#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#pragma once

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

time_t HttpConvertDateStringToTime(const std::string& str);

class HttpResponse;

typedef std::map<std::string, std::string> HttpHeaders;

typedef boost::function<void(const HttpHeaders& headers)> HttpHeadersHandler;
typedef boost::function<void(const uint8_t* data, size_t length)> HttpContentHandler;
typedef boost::function<void(const boost::system::error_code& ec, const HttpResponse& response)> HttpRequestHandler;

/// Параметры HTTP запроса
class HttpRequest {
public:
	enum Method {
		HEAD,
		GET,
		POST
	};

	enum {
		/* milliseconds */
		kDefaultOpenTimeout = 2 * 1000,
		kDefaultReadTimeout = 5 * 1000
	};

	HttpRequest();

	explicit HttpRequest(const std::string& url);

	HttpRequest& SetMethod(Method method);
	Method GetMethod() const;

	HttpRequest& SetUrl(const std::string& url);
	HttpRequest& SetUrl(std::string&& url);
	const std::string& GetUrl() const;

	HttpRequest& SetContent(const std::string& content);
	HttpRequest& SetContent(std::string&& content);
	const std::string& GetContent() const;

	HttpRequest& SetContentType(const std::string& content_type);
	HttpRequest& SetContentType(std::string&& content_type);
	const std::string& GetContentType() const;

	HttpRequest& SetUserAgent(const std::string& user_agent);
	HttpRequest& SetUserAgent(std::string&& user_agent);
	const std::string& GetUserAgent() const;

	HttpRequest& AppendHeader(const std::string& name, const std::string& value);
	HttpRequest& AppendHeader(std::string&& name, std::string&& value);
	const HttpHeaders& GetHeaders() const;

	HttpRequest& SetOpenTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetOpenTimeout() const;

	HttpRequest& SetReadTimeout(const boost::posix_time::time_duration& timeout);
	const boost::posix_time::time_duration& GetReadTimeout() const;

	HttpRequest& SetMaxRedirects(size_t max_redirects);
	size_t GetMaxRedirects() const;

private:
	Method _method = GET;

	std::string _url;
	std::string _content;
	std::string _contentType;
	std::string _userAgent;

	HttpHeaders _headers;

	boost::posix_time::time_duration _openTimeout = boost::posix_time::milliseconds(kDefaultOpenTimeout);
	boost::posix_time::time_duration _readTimeout = boost::posix_time::milliseconds(kDefaultReadTimeout);

	size_t _maxRedirects = std::numeric_limits<size_t>::max();
};

/// Параметры HTTP ответа
class HttpResponse {
public:
	HttpResponse() = default;

	void SetHeaders(const HttpHeaders& headers);
	void SetHeaders(HttpHeaders&& headers);

	const HttpHeaders& GetHeaders() const;
	const std::string& GetHeader(const std::string& name) const;

	void SetContentType(const std::string& content_type);
	void SetContentType(std::string&& content_type);
	const std::string& GetContentType() const;

	HttpResponse& AppendContent(const uint8_t* data, size_t count);

	const uint8_t* GetContent() const;
	size_t GetContentLength() const;

private:
	HttpHeaders _headers;
	std::string _contentType;
	std::vector<uint8_t> _content;
};

class BasicHttpClientService : public boost::asio::io_service::service {
public:
	static boost::asio::io_service::id id;

	explicit BasicHttpClientService(boost::asio::io_service& io_service);

	~BasicHttpClientService();

	// Implementation содержит в себе состояние каждого отдельного объекта-клиента этого сервиса
	class Implementation;

	typedef boost::shared_ptr<Implementation> implementation_type;

	void construct(implementation_type& impl);
	void destroy(implementation_type& impl);

	boost::asio::io_service& GetWorkIoService(implementation_type& impl);

	void OnReadHeaders(implementation_type& impl, HttpHeadersHandler handler, boost::system::error_code& ec);
	void OnAppendContent(implementation_type& impl, HttpContentHandler handler, boost::system::error_code& ec);

	HttpResponse Query(implementation_type& impl, const HttpRequest& request, boost::system::error_code& ec);
	void QueryAsync(implementation_type& impl, const HttpRequest& request, HttpRequestHandler handler);
	void Cancel(implementation_type& impl);

	bool IsRunning(implementation_type& impl);
	size_t GetBytesTransferred(implementation_type& impl);
	size_t GetContentLength(implementation_type& impl);

private:
	void shutdown_service() {
	}

private:
	boost::asio::io_service _http_io_service;
	std::unique_ptr<boost::asio::io_service::work> _http_work;
	boost::thread _http_thread;
};

template <typename Service>
class BasicHttpClient : public boost::asio::basic_io_object<Service> {
public:
	explicit BasicHttpClient(boost::asio::io_service& io_service)
		: boost::asio::basic_io_object<Service>(io_service)
	{
	}

	~BasicHttpClient() {
		// Если объект уничтожается раньше завершения асинхронной загрузки, то загрузка отменяется
		Cancel();
	}

	/// Возвращает io_service рабочего потока
	boost::asio::io_service& GetWorkIoService() {
		return this->get_service().GetWorkIoService(this->get_implementation());
	}

	/// Контекст вызова обработчика будет зависеть от варианта запроса:
	/// - В случае Query() он будет вызван из того же потока
	/// - В случае QueryAsync() он будет вызван из *другого* потока
	void OnReadHeaders(HttpHeadersHandler handler) {
		boost::system::error_code ec;
		this->get_service().OnReadHeaders(this->get_implementation(), handler, ec);
		boost::asio::detail::throw_error(ec);
	}

	/// Контекст вызова обработчика будет зависеть от варианта запроса:
	/// - В случае Query() он будет вызван из того же потока
	/// - В случае QueryAsync() он будет вызван из *другого* потока
	void OnReadHeaders(HttpHeadersHandler handler, boost::system::error_code& ec) {
		this->get_service().OnReadHeaders(this->get_implementation(), handler, ec);
	}

	/// При установленном обработчике добавления контента данные в объект @HttpResponse добавляться не будут
	/// Контекст вызова обработчика будет зависеть от варианта запроса:
	/// - В случае Query() он будет вызван из того же потока
	/// - В случае QueryAsync() он будет вызван из *другого* потока
	void OnAppendContent(HttpContentHandler handler) {
		boost::system::error_code ec;
		this->get_service().OnAppendContent(this->get_implementation(), handler, ec);
		boost::asio::detail::throw_error(ec);
	}

	/// При установленном обработчике добавления контента данные в объект @HttpResponse добавляться не будут
	/// Контекст вызова обработчика будет зависеть от варианта запроса:
	/// - В случае Query() он будет вызван из того же потока
	/// - В случае QueryAsync() он будет вызван из *другого* потока
	void OnAppendContent(HttpContentHandler handler, boost::system::error_code& ec) {
		this->get_service().OnAppendContent(this->get_implementation(), handler, ec);
	}

	HttpResponse Query(const HttpRequest& request) {
		boost::system::error_code ec;
		auto response = this->get_service().Query(this->get_implementation(), request, ec);
		boost::asio::detail::throw_error(ec);
		return response;
	}

	HttpResponse Query(const HttpRequest& request, boost::system::error_code& ec) {
		return this->get_service().Query(this->get_implementation(), request, ec);
	}

	void QueryAsync(const HttpRequest& request, const HttpRequestHandler& handler) {
		this->get_service().QueryAsync(this->get_implementation(), request, handler);
	}
	
	void Cancel() {
		this->get_service().Cancel(this->get_implementation());
	}

	bool IsRunning() {
		return this->get_service().IsRunning(this->get_implementation());
	}

	size_t GetBytesTransferred() {
		return this->get_service().GetBytesTransferred(this->get_implementation());
	}

	size_t GetContentLength() {
		return this->get_service().GetContentLength(this->get_implementation());
	}
};

typedef BasicHttpClient<BasicHttpClientService> HttpClient;

#endif // __HTTPCLIENT_H__

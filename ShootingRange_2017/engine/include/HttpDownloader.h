#ifndef __HTTPDOWNLOADER_H__
#define __HTTPDOWNLOADER_H__

#pragma once

namespace IO {
	class OutputStream;
}

class HttpResponse;

class HttpConnection : public RefCounter {
public:
	virtual const std::string& GetUrl() const = 0;
	virtual size_t GetBytesTransferred() const = 0;	
	virtual size_t GetContentLength() const = 0;

	virtual bool IsRunning() const = 0;	
	virtual void Cancel() = 0;
};

typedef boost::intrusive_ptr<HttpConnection> HttpConnectionPtr;

typedef boost::function<void(const HttpConnection* connection, const HttpResponse& response, const boost::system::error_code& ec)> HttpDownloadResultHandler;

class HttpDownloader {
public:
	explicit HttpDownloader(boost::asio::io_service& io_service);

	HttpDownloader(const HttpDownloader&) = delete;
	HttpDownloader& operator=(const HttpDownloader&) = delete;

	void StartDownload(const std::string& url, const IO::OutputStreamPtr& stream, HttpDownloadResultHandler handler);
	void StartDownload(const std::string& url, const std::string& filename, HttpDownloadResultHandler handler);

	void StartDownload(const std::string& url, const IO::OutputStreamPtr& stream, const boost::posix_time::time_duration& timeout, HttpDownloadResultHandler handler);
	void StartDownload(const std::string& url, const std::string& filename, const boost::posix_time::time_duration& timeout, HttpDownloadResultHandler handler);

	bool IsRunning() const;
	void Cancel();

private:
	boost::asio::io_service& _io_service;

	typedef std::list<HttpConnectionPtr> Connections;
	mutable Connections _connections;

public:
	// Iterator interface for range-for loop
	Connections::iterator begin() { return _connections.begin(); }
	Connections::iterator end() { return _connections.end(); }
	Connections::const_iterator cbegin() { return _connections.cbegin(); }
	Connections::const_iterator cend() { return _connections.cend(); }
};

#endif // __HTTPDOWNLOADER_H__

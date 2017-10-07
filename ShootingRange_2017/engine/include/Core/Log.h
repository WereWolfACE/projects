#ifndef _LOG_H_
#define _LOG_H_

#include "IO/Stream.h"
#include "RefCounter.h"
#include "ThreadSupport.h"

namespace Log {

///
/// Перечисляет уровни логгирования
///
enum class Severity {
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

std::ostream& operator<<(std::ostream& o, Severity severity);

class LogSink : public RefCounter {
public:
	virtual void WriteMessage(Severity severity, const std::string& message) = 0;
};

typedef boost::intrusive_ptr<LogSink> LogSinkPtr;

class Log
{
public:
	Log();
	
	void AddSink(LogSink* sink);

	void RemoveSink(LogSink* sink);
	
	void RemoveAllSinks();
	
	void SetMinSeverityLevel(Severity severity);
	
	Severity GetMinSeverityLevel() const;
	
	void WriteMessage(Severity severity, const std::string& message);
	
	void WriteDebug(const std::string& msg);
	
	void WriteInfo(const std::string& msg);
	
	void WriteWarn(const std::string& msg);
	
	void WriteError(const std::string& msg);
	
	void WriteFatal(const std::string& msg);

private:
	friend class TextFileLogSink;
	friend class HtmlFileLogSink;

	static boost::asio::io_service& io();
	
private:
	typedef std::vector<LogSinkPtr> Sinks;	

	mutable MUTEX_TYPE _writeMutex;
	
	Sinks _sinks; ///< Коллекция приёмников сообщений	

	Severity _severity; ///< Уровень серьёзности
};

extern Log log;

void Debug(const std::string& msg);
void Info(const std::string& msg);
void Warn(const std::string& msg);
void Error(const std::string& msg);
void Fatal(const std::string& msg);

///////////////////////////////////////////////////////////////////////////////
// Log sinks

// Логирование в обычный текстовый файл. Иногда может быть удобнее, чем html, например
// в том случае, когда логи отсылаются в HockeyApp или подобный сервис
class TextFileLogSink : public LogSink {
public:
	TextFileLogSink(const std::string& filename, bool recreate);
	~TextFileLogSink();

	virtual void WriteMessage(Severity severity, const std::string& message) override;

protected:
	void WriteFileString(const std::string& str);

private:
	IO::OutputStreamPtr _stream;
};
	
/// Logging to html formatted file
class HtmlFileLogSink : public TextFileLogSink {
public:
	HtmlFileLogSink(const std::string& filename, bool recreate);		
	~HtmlFileLogSink();
	
	virtual void WriteMessage(Severity severity, const std::string& message) override;
};

/// Logging to debug output
class DebugOutputLogSink : public LogSink {
public:
	virtual void WriteMessage(Severity severity, const std::string& message) override;
};

} // namespace Log

#endif


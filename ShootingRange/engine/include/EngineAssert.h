#ifndef __ENGINE_ASSERT_H__
#define __ENGINE_ASSERT_H__

#include "Platform/TargetPlatforms.h"

#include <string>

#include <boost/function.hpp>

std::string GetStackTrace();

bool IsRunningUnderDebugger();
void BreakIntoDebugger();

typedef boost::function<void(const std::string&, const std::string&, const char*, int)> AssertionHandler;
typedef boost::function<void(const std::string&)> AssertionSendToHandler;

/// При установленном AssertionHandler вызов AssertionFailed приводит к выполнению только этого обработчика
void SetAssertionHandler(AssertionHandler handler);
AssertionHandler GetAssertionHandler();

/// При установленном AssertionSendToHandler в окно ассерта добавляется кнопка `Send To...`
void SetAssertionSendToHandler(AssertionSendToHandler handler);
AssertionSendToHandler GetAssertionSendToHandler();

void AssertionFailed(const std::string& message, const std::string& expression, const char* file, int line);

#if defined(ENABLE_ENGINE_ASSERT)

#define Assert(expr) do { if (!(expr)) { AssertionFailed("Assertion failed", #expr, __FILE__, __LINE__); } } while (false)
#define Assert2(expr, message) do { if (!(expr)) { AssertionFailed(message, #expr, __FILE__, __LINE__); } } while (false)
#define Halt(message) AssertionFailed("Halt occured", message, __FILE__, __LINE__)

#else // !ENABLE_ENGINE_ASSERT

namespace Log {
void Error(const std::string& msg);
}

#define Assert(expr) ((void)0)
#define Assert2(expr, message) ((void)0)
#define Halt(message) Log::Error(message)

#endif // ENABLE_ENGINE_ASSERT

#endif // __ENGINE_ASSERT_H__

#ifndef __MARKETING_SMSDIALOG_H__
#define __MARKETING_SMSDIALOG_H__

#include "types.h"
#include <boost/function.hpp>

namespace Marketing
{

class SmsDialog
{
public:

	// на андроиде возращается NOT_SUPPORTED, если отправка смс не поддерживается,
	// и SUCCEEDED в остальных случаях, т.к. нет возможности проверить статус действия
	struct SendResult {
		enum Type {
			SUCCEEDED = 0,
			FAILED,
			CANCELED,
			NOT_SUPPORTED
		};
	};

	typedef boost::function<void(SendResult::Type result)> CallbackType;

	SmsDialog();

	void SetMessageBody(const std::string &textId);

	void SetCallback(CallbackType callback);

	void Show();

private:

	bool _shown;

	std::string _message;

	CallbackType _callback;

};

}

#endif //__MARKETING_SMSDIALOG_H__
#ifndef __MARKETING_EMAILDIALOG_H__
#define __MARKETING_EMAILDIALOG_H__

#include "types.h"
#include <boost/function.hpp>

namespace Marketing
{

class EmailDialog
{
public:

	// на андроиде возращается NOT_CONFIGURED, если отправка почты не работает (что маловероятно),
	// и SUCCEEDED в остальных случаях, т.к. нет возможности проверить статус действия
	struct SendResult {
		enum Type {
			SUCCEEDED = 0,
			FAILED,
			CANCELED,
			NOT_CONFIGURED,
			DRAFT_SAVED
		};
	};

	struct Extra {
		enum Type {
			LANG_GAME_INFO = 0,
			NO_INFO
		};
	};

	struct MessageType {
		enum Type {
			HTML = 0,
			PLAIN_TEXT
		};
	};

	typedef boost::function<void(SendResult::Type result, const std::string &errorReason, const std::string &errorDescription)> CallbackType;

	EmailDialog();

	void AddRecipient(const std::string &rec);

	void SetSubject(const std::string &textId);

	void SetMessageBody(const std::string &textId, MessageType::Type type, Extra::Type extra);

	void AddAttachment(const std::string &path, const std::string &mime);

	void SetCallback(CallbackType callback);

	void Show();

private:

	typedef std::vector<std::string> RecipientList;

	typedef std::map<std::string, std::string> Attachments;

	bool _shown;

	std::string _subject;

	std::string _message;

	MessageType::Type _msgType;

	CallbackType _callback;

	RecipientList _recipients;

	Attachments _attachments;

	friend void RunDialogCallback(EmailDialog *dialog, EmailDialog::SendResult::Type result);

};

}

#endif //__MARKETING_EMAILDIALOG_H__
#ifndef __MARKETING_LOCALNOTIFICATION_H__
#define __MARKETING_LOCALNOTIFICATION_H__

namespace Marketing
{

class LocalNotifications
{
public:

	struct Sound {
		enum Type {
			DEFAULT = 0,
            CUSTOM,
			NONE
		};
	};

	static void Init();
    
    static void Add(int secondsToStart, const std::string &textId, Sound::Type sound = Sound::DEFAULT, const std::string &customSound="");

	// TODO добавить функций с другими формами задания времени

	static void CancelAll();

	static void ScheduleAll();

    static bool IsEnabled();
    
    static bool IsSoundEnabled();
    
};

}

#endif // __MARKETING_LOCALNOTIFICATION_H__

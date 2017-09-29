#ifndef _GCINTERFACE_H
#define _GCINTERFACE_H

#include <vector>
#include <string>
#include <boost/function.hpp>
/**
 * Поддерживается ли геймцентр девайсом.
 * Если поддерживается, то стоит показывать некотоные контролы, 
 * например, кнопки "достижения" и/или "рейтинг"
 */
bool GCSupported();
	
/**
 * Залогинен ли игрок в геймцентр
 */
bool GCAuthenticated();

/**
 * Устанавливается в >0 если во время логина произошла ошибка,
 * или игрок отказался логиниться, или ваще геймцентра нету.
 * Установлена в <0 если игрок ещё не залогинился или не подтвердил
 * желание использовать GameCenter в игре.
 * Устанавливается в 0 после того как произошёл логин.
 */
int GCAuthenticationFailed();
	
/**
 * Аутентифицирует текущего геймцентровского игрока.
 * Проверяет, поддерживается ли геймцентр и предлагает пользователю залогиниться/создать аккаунт.
 * Функцию следует вызывать после загрузки иерархии вьюшек
 * (в конце applicationDidFinishLaunching)
 * иначе есть риск, что сообщение о логине просто не выведется
 */
void GCAuthenticateLocalPlayer();

/**
 Список действий.
 */
struct GCOperation {
	enum Type {
		LOGIN_FINISHED = 0,
		ACHIEVEMENTS_LOAD_FINISHED,
		LOAD_FRIENDS_SUCCEEDED,
		LOAD_FRIENDS_FAILED
	};
};

/**
 Колбек о завершении различных действий.
 */
typedef boost::function<void(GCOperation::Type)> GCMainCallback;
void GCSetMainCallback(GCMainCallback cb);

/**
 * Устанавливает прогресс достижения геймцентровского ачивмента и сразу пытается отправить.
 * Если отправить не удалось, сохраняет до лучших времен.
 * Если установить прогресс, меньший либо равный прогрессу, установленному раньше,
 * ничего не произойдет.
 * Если прогресс максимальный и он был достигнут впервые, то покажется
 * всплывающее окно с названием ачивмента (однако если до этого не удастся получить
 * из геймцентра список ачивментов с названиями, ничего не покажется).
 * @param achievementID   Идентификатор ачивмента
 * @param progress        Прогресс ачивмента (от 0.0f до 1.0f)
 */
void GCSetAchievementProgress(const char* achievementID, float progress);

/**
 * Сохранить данные на диск
 * Это следует прописать в GameInfo::Save(),
 * делать это чаще вряд ли стоит, в худшем случае, достижение будет показано
 * более одного раза
 */
void GCFlushData();

/**
 * Установить счет для категории
 * @param categoryID  идентификатор категории
 * @param score       счет в этой категории
 */
void GCSetScore(const char* categoryID, int64_t score);

/**
 * Установить путь к графике для геймцентра.
 * Например, в 80 днях он такой: "base/textures/GameCenter/".
 * Вызывать можно в любой момент перед получением достижения, например, в функции main.
 */
extern "C" {
	void GCSetPathToGraphics(const char* path);
}
	
/// Вернуть alias игрока геймцентра, либо пустую строку.
std::string GCPlayerName();

/// Вернуть уникальный идентификатор пользователя (либо "").
std::string GCPlayerId();

/**
 * Выключить всю поддержку геймцентра.
 * Все функции перестанут делать что-то содержательное.
 * Может пригодиться в лайт-верии
 */
void GCTurnOff();
	
/**
 * Засабмитить данные в геймцентр.
 * Дело в том, что сабмитить их в тот момент, когда мы их получили,
 * плохо, т.к. наблюдается заметные тормоза в момент отправки
 */
void GCSubmitData();
	
/**
 * Обнулить все данные геймцентра игрока
 * и послать соответствующий запрос в геймцентр
 * (еще не реализовано)
 */
void GCResetData();

/// Загрузить список друзей из GameCenter (асинхронно).
bool GCLoadFriends();

bool GCWaitingForFriends();

bool GCHasFriends();

struct GCFriend
{
	std::string gcId;
	std::string name;
	std::string displayName; // Реальное имя игрока, показывается только друзьям.
	std::string picUrl; // на ios не заполняется, картинку можно загрузить только с помощью GCLoadUserPic
};
	
/// Список друзей из GameCenter.
std::vector<std::string> GCFriendIds();

GCFriend GCGetFriend(const std::string& gcId);

// Загрузить изображение игрока playerId
// По завершении вызывается callback
void GCLoadUserPic(const std::string& playerID);
/**
 Колблек о загрузке картинки какого-либо игрока
 Сигнатура функции: void FuncName(const std::string& gameCenterUserID, const std::string& pathToImage)
 */
typedef boost::function<void(const std::string&, const std::string&)> GCLoadUserPicCallback;
void GCSetLoadedUserPicCallback(GCLoadUserPicCallback cb);

void GCShowAchievements();

void GCShowLeaderboard(const char* categoryID = NULL);
#endif // _GCINTERFACE_H

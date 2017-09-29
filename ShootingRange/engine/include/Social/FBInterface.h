#ifndef _FBINTERFACE_H
#define _FBINTERFACE_H

#include <string>
#include <vector>
#include <map>
#include <boost/function.hpp>

namespace FB
{

	struct Action {
		enum Type {
			LOGIN_COMPLETED = 0, // former lua:OnFBLoggedIn, успешный логин
			LOGIN_FAILED, // former lua:OnFBLoginCancelled, неуспешный логин
			LOGIN_FOR_PUBLISH_COMPLETED, // former lua:OnFBLoginFinished, успешный логин на запись, приходит после LOGIN_COMPLETED
			LOGIN_FOR_PUBLISH_FAILED, // неуспешный логин на запись, приходит после LOGIN_COMPLETED
			LOGIN_FOR_PUBLISH_CANCELED, // логин на запись был отменен пользователем
			LOGOUT, // former lua:OnFBLogout, выход
			USERINFO_AVAILABLE, // former lua:OnFBUserInfo, скачали информацию о пользователе
			USERINFO_FAILED, // former lua:OnFBUserInfoFail, информацию о пользователе не удалось скачать
			FRIENDSINFO_AVAILABLE, // former lua:OnFBFriendsInfo, появилась новая информация о друзьях
			FRIENDSINFO_FAILED, // former lua:OnFBFriendsInfoFail, информацию о друзьях не удалось скачать
			INVITABLEFRIENDS_AVAILABLE, // появилась информация о друзьях, которых можно пригласить
			INVITABLEFRIENDS_FAILED, // не удалось получить информацию о друзьях, которых можно пригласить
			DIALOG_POSTED, // сообщение с диалогом опубликовано
			DIALOG_CANCELED, // сообщение с диалогом не удалось опубликовать
			REQUEST_SENT, // запрос отправлен
			REQUEST_SENT_ERROR, // ошибка при отправке запроса
			REQUEST_FAILED, // отправка запроса не удалась
			REQUEST_DIALOG_SHOWN, // показывается диалог отправки запроса
			REQUEST_DIALOG_CLOSED, // диалог отправки запроса закрыт
			APPREQUEST_AVAILABLE, // скачали информацию о доступных запросах
			APPREQUEST_FAILED, // информацию о доступных запросах не удалось скачать
			OS_VERSION_NOT_SUPPORTED, // на данной версии iOS работать не будем
			LIKESINFO_AVAILABLE, // доступна информация от Facecbook о лайке страницы игры
			LIKESINFO_FAILED // не удалось получить информацию о лайках, например, пользователь не дал разрешния получение этой информации
		};
	};

	typedef boost::function<void(Action::Type action)> CallbackType;
	typedef boost::function<void()> LogInPublishCallbackType;

	void SetTimeout(int timeoutInSeconds);
	///< задать размеры изображений аватаров пользователя и друзей
	///< нужно вызывать перед запросами RequestUserInformation, RequestFriendsInformation, RequestInvitableFriendsInformation
	///< картинки по ссылкам всё равно будут тех размеров, каких захочет facebook, а не именно этих
	void SetUserPicturesSize(int width, int height);
	void InitWithCallback(CallbackType callback);
	void LogIn(); ///< Залогиниться.
	void LogOut(); ///< Вылогиниться.
	bool IsLoggedIn(); ///< Проверить залогинен ли.
	bool CanPublish(); ///< Есть ли права на запись.
	bool CanCheckLike(); ///< Можно ли проверить like.
	bool AuthInProcess(); ///< Инициализация Facebook в процессе.

	bool LoadingUserInfo(); // Загрузка информации об игроке в процессе.
	bool IsWaitingForFriends(); ///< Ждем информацию о друзьях.

	std::string UserName(); // Полное имя пользователя.
	std::string UserId(); // Идентификатор пользователя (числовой).
	std::string UserPicUrl(); // Url аватарки залогиненного пользователя.
	std::string UserFirstName(); // Имя пользователя.

	bool RequestUserInformation(); // Запросить информацию о текущем игроке. Возвращает false, если ничего не делает.
	bool RequestFriendsInformation(int totalRetrieveLimit = 0); // Запросить информацию о друзьях. Возвращает false, если ничего не делает.
	bool RequestAppRequestsInformation(int totalRetrieveLimit = 0); // Запросить информацию о "запросах приложения". Возвращает false, если ничего не делает.

	// Внимание! : только для игр с Facebook Canvas и Graph API с версии v2.0
	bool RequestInvitableFriendsInformation(int totalRetrieveLimit = 0); // Запросить информацию о друзьях, которых можно пригласить.
	
	bool RequestLikesInformation();
		// Запросить информацию о лайках. Если пользователь не вошел, то вернет false.
		// Если нет разрешения, то оно будет запрошено (может появиться окно фейсбука) и вернется true. После получения разрешения запрос начнется автоматически.s
		// Если есть разрешение, то начнется запрос и вернется true.

	bool IsGameLiked();
	
	struct Friend
	{
		std::string fbId;
		std::string name;
		std::string first_name;
		std::string pic_url;
	};
	
	struct InvitableFriend
	{
		std::string inviteId;	// invite token
		std::string name;
		std::string pic_url;
	};
	
	struct AppRequest {
		std::string requestId;
		std::string senderId;
		std::string message;
		std::string data;
	};

	std::vector<std::string> FriendIds();
	Friend GetFriend(const std::string& fbId);
	
	std::vector<std::string> InviteIds();
	InvitableFriend GetInvitableFriend(const std::string& inviteId);

	// инициализация, нужно вызывать на самом старте (в конструкторе EngineAppDelegate)
	void Initialize();

	void FetchDeferredDeepLink();

	void HandleDidBecomeActive();

	typedef std::map<std::string, std::string> Parameters;
	void PostWithDialog(const Parameters &params); // deprecated
	void PostOnWall(const Parameters &params);
	void SendRequest(const Parameters &params);

	// example PostToOpenGraph("me/townshipgame:ACTION", "http://xml.playrix.com/township_ios/og1.php?object=OBJ&lang=LANG&...OTHER_PARAMS", "OBJ");
	void PostToOpenGraph(const std::string& actionWithPrefix, const std::string& objurl, const std::string& objname);
	
	std::vector<AppRequest> GetAppRequests();
	void DeleteAppRequest(const std::string &requestid);

    // Отправка событий для аналитики
    void LogEventActivateApp();
    void LogEventFBLogin();
    void LogEventCompletedTutorial();
    void LogEventAchievedLevel(int level);
    void LogEventPurchased(float value, const std::string& currency);
	void LogEventAchievement(const std::string& achievement);
	void LogEventCustom(const std::string& event, const FB::Parameters& parameters);
}
#endif // _FBINTERFACE_H

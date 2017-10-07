#ifndef _SWRVE_WRAPPER_H_
#define _SWRVE_WRAPPER_H_

#include <string>
#include <map>
#include <boost/function.hpp>
#include "ios/Classes/StrongRefObjC.h"

// Оболочка для swrve. Отсеиванием читеров пока должна заниматься игра.

namespace SwrveWrapper
{
#ifndef __OBJC__
	struct SKPaymentTransaction {};
#endif

	struct DownloadStatus {
		enum Type {
			SUCCEEDED = 0, // успешно скачали, данные во втором параметре
			FAILED, // данные скачать не получилось
			USER_CHANGED // поменялся пользователь, данные есть, но использовать их нельзя
		};
	};

	struct Reward {
		enum Type {
			ITEM = 0, // покупка предмета
			CURRENCY // покупка валюты
		} type; //< тип покупки - предмет или валюта
		
		std::string subtype; //< уточнение типа, например, "cash" и "coins" для CURRENCY
		
		int quantity; //< количество покупаемых предметов, например, 100 "coins"
		
		Reward(): type(CURRENCY), quantity(1)
		{};
		Reward(Type type, const std::string& subtype, int quantity):
			type(type), subtype(subtype), quantity(quantity)
		{};
	};

	struct InAppPurchaseInfo {
		int productQuantity; //< количество продуктов в покупке, сейчас всегда предполагается 1

		std::string currencyCode; //< код валюты
		float productPrice; //< цена в этой валюте

		std::string receipt; //< для Android: отчет магазина о покупке
		std::string receiptSignature; // для Android: цифровая подпись отчета для валидации

		std::vector<Reward> rewards;

		StrongRefObjC<SKPaymentTransaction> transaction;

		InAppPurchaseInfo()
			: productQuantity(1)
			, productPrice(0.0f)
		{}

		InAppPurchaseInfo(const std::string &currency_code, float price,
			Reward::Type reward_type, const std::string &reward_subtype, int rewardQuantity_,
			const std::string &receipt_ = "", const std::string &receiptSignature_ = ""
		)
			: productQuantity(1)
			, currencyCode(currency_code)
			, productPrice(price)
			, receipt(receipt_)
			, receiptSignature(receiptSignature_)
		{
			rewards.emplace_back(reward_type, reward_subtype, rewardQuantity_);
		}
	};

	typedef boost::function< std::map<std::string, std::string>() > GetUserInfoCallback;
		// для получения из игры изменившейся общей статистики по игроку (swrve.userUpdate)

	typedef boost::function< void(DownloadStatus::Type status, const std::string &) > DownloadResourceCompleteCallback;
		// для обработки скачанных ресурсов и аб-тестов

	void Init(int appId, const std::string &apiKey, const std::string &storeId, const std::string &gcmId,
		GetUserInfoCallback getUserInfoCallback, DownloadResourceCompleteCallback onDownloadResourceCompleteCallback);
		// настройки для подключения

	std::string GetActiveUser();
		// получить id активного пользователя

	void SetActiveUser(const std::string &userId);
		// задать id активного пользователя

	void DownloadUserResources();
		// скачать ресурсы для активного пользователя (A/B-тесты)

	/// Для ios в transaction необходимо передать SKPaymentTransaction
	void TrackIAP(const std::string &productId, const InAppPurchaseInfo &info); // swrve.iap - inapp-покупка
	void TrackPurchase(const std::string &item, int cost, const std::string& ctype, int quantity); // swrve.purchaseItem - покупка предметов за игровую валюту
	void TrackCurrencyGiven(const std::string &ctype, int amount); // swrve.currencyGiven - начисление игровой валюты (помимо IAP)
	void TrackEvent(const std::string &event_name, const std::string& payload_val); //swrve.event, payload в виде json-строки - просто событие
	
	void SendQueuedEvents();
		// отправить накопленную статистику на сервер, период отправки определяет игра

	void ClearPendingEvents(); // удаляет отложенные события (не попавшие в swrve из-за незаданного userId)
	void ClearCachedEvents(); // удаляет кэш событий с диска (события, переданные в swrve api, но не отправленные на сервер), вызывать до Init

#if defined(ENGINE_TARGET_IPHONE) && defined(__OBJC__)
	// Эти функции нужно вызывать в AppDelegate
	void OnApplicationDidEnterBackground();
	void OnApplicationDidBecomeActive();
	void OnApplicationWillTerminate();
	void OnApplicationDidRegisterForRemoteNotificationsWithDeviceToken(NSData *deviceToken);
#endif
}

#endif /* _SWRVE_WRAPPER_H_ */

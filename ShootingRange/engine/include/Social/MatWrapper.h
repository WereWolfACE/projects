#pragma once

#include <string>
#include <vector>

namespace MatWrapper
{
	struct MatPurchaseInfo
	{
		// Имя события, для разных плаформ могут быть разные
		std::string eventName;
		// Название товара
		std::string productId;
		// Уникальный идентификатор покупки
		std::string orderId;
		// Код валюты
		std::string currencyCode;
		// Количество
		int quantity;
		// Цена за штуку
		float unitPrice;
		// Данные для проверки покупки (используется только на ios)
		std::vector<char> receipt;
	};

	// Инициализация сервиса
	// debugLog - вывод в лог информации о работе службы (успехе или неудаче при отсылке и пр.)
	// defferedLinking - отслеживание ссылок, пришедших до установки приложения
	// packageName - идентификатор приложения, нужно задавать только если отличается от реального
	void Init(const std::string& advertiserId, const std::string& conversionKey, bool debugLog, bool defferedLinking, const std::string& packageName);

	bool IsInitialized();

	// Установка UserId
	void SetUserId(const std::string &userId);

	// обработка ссылок
	// на ios должна вызываться из application:openURL
	// на android
	void HandleOpenUrl(const std::string &url, const std::string &sourceApp);

	// Отслеживание события по имени
	void TrackEvent(const std::string &eventName);

	// Отслеживание покупки
	void TrackPurchase(const MatPurchaseInfo &purchase);

	// Отслеживание достижения уровня
	void TrackAchievedLevel(int level);

	// Отслеживание достижения
	void TrackAchievementUnlocked(const std::string &achievementName);

	// Отслеживание сессии (вызывается при старте игры или возврате в игру из паузы)
	void TrackSession();
}

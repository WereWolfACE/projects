//
//  MicroTransactions.h
//  fishdom_ipad
//
//  Created by vasiliym on 21.02.11.
//  Copyright 2011 Playrix Ent. All rights reserved.
//
#ifndef _MICROTRANSACTIONS_H
#define _MICROTRANSACTIONS_H

#include <string>
#include <vector>

#ifdef __OBJC__
	#import <StoreKit/SKPaymentTransaction.h>
#endif

// Покупки работают так:
//
// При запуске программы нужно инициализировать магазин.
// Для этого в метод InitStore передаётся список всех идентификаторов покупок.
// Магазин асинхронно запрашивает объекты всех продуктов с сервера.
// Объекты продуктов нужны, чтобы затем совершать покупки.
//
// Если инициализация не закончена, то действия MakePurchase и RestorePurchases
// будут отложены и автоматически выполнены при успешном завершении инициализации.
//
// Если инициализация завершилась неуспешно (например, при запуске игры не было интернета),
// то при следующем вызове MakePurchase (или RestorePurchases) будет новая попытка инициализации магазина.
//
// Большинство операций - асинхронные.
//
// О событиях информация приходит через Delegate.


#ifdef __cplusplus

namespace InApp
{

class Delegate
{
public:

	virtual ~Delegate() {}

	virtual void OnInitialized() = 0; // инициализация магазина успешно завершена.
	virtual void OnInitializationFailed(void *error) = 0; // инициализация магазина завершилась с ошибкой, реальный тип ошибки NSError*
	virtual void OnInvalidIds(const std::vector<std::string> ids) = 0; // при запросе продукта выяснилось, что эти Id невалидны.

	virtual void OnRestorePurchasesFinished() = 0; // все покупки восстановлены.
	virtual void OnRestorePurchasesFailed(void *error) = 0; // восстановление покупок завершилась с ошибкой, реальный тип ошибки NSError*
	virtual void OnRestorePurchasesCancelled() = 0; // восстановление покупок отменено

	virtual void OnPurchaseFinished(const std::string &productId) = 0; // покупка совершена, в т.ч. успешно прошла валидация
	virtual void OnPurchaseFailed(const std::string &productId, void *error) = 0; // // покупка не удалась, реальный тип ошибки NSError*
	virtual void OnPurchaseCancelled(const std::string &productId) = 0;  // покупка отменена
	virtual void OnTransactionRemoved(const std::string &productId) = 0; // удаление транзакции из очереди (здесь предполагается обновление UI; возможно, не нужно)

// События верификации.
// Если включена верификация, то следующие события должны быть обработаны. InApp::FinishPurchase вызывать вручную не надо.
	virtual void OnVerificationStarted(const std::string &productId) = 0;
	virtual void OnVerificationSucceeded(const std::string &productId) = 0;
	virtual void OnVerificationFailed(const std::string &productId, void *error) = 0; // реальный тип ошибки NSError*, error!=nil при ошибке подключения
};

//
void SetDelegate(Delegate *dlgt);


/// Инициализация магазина.
/// Это асинхронная операция: системе требуется загрузить объекты покупок с сервера.
/// autoRestore - надо ли обращаться в iTunes за списком уже совершённых покупок (требует ввода пароля)
void InitStore(const std::vector<std::string>& identifiers, bool autoRestore = false);

#ifdef __OBJC__

NSDictionary * FindPurchase(NSString * productId);
SKPaymentTransaction * FindTransaction(const std::string& productId);

#endif

/// Инициализировался ли магазин.
bool StoreIsInited();

/// Инициализация магазина в процессе?
bool IsInitInProcess();

void ReinitStore();

void Release();

/// Совершить покупку (асинхронно).
/// Требует ввода пароля.
/// Возвращает true если покупка была успешно поставлена в очередь.
bool MakePurchase(const std::string& _id, const std::string &payload = "", int quantity = 1);

/// Включить или выключить верификацию.
void SetVerificationRequired(bool b);

bool IsVerificationInProgress();

/// Возвращает false если пользователь запретил покупки, или если магазин ещё не инициализирован.
bool CanPurchase();

/// Восстановить все покупки (асинхронно).
/// Требует ввода пароля, поэтому обычно вызывается при нажатии кнопки "Already Paid".
/// Возвращает true, если процесс запущен.
bool RestorePurchases();

void FinishPurchase(const std::string& productId);

/// Возвращает ссылку на вектор с идентификаторами продуктов.
/// Это те же идентификаторы, которые были переданы в InitStore.
const std::vector<std::string>& GetProducts();

/// Информация по продуктам.
std::vector<std::string> GetActiveProducts(); ///< Возвращает id продуктов, успешно полученных с сервера.
bool HasProduct(const std::string& _id); ///< Возвращает, есть ли такой продукт.
bool HasActiveProduct(const std::string& _id); ///< Возвращает, получен ли с сервера этот продукт.
int ProductsCount(); ///< Кол-во продуктов в магазине.
std::string ProductName(int n); ///< n - порядковый номер продукта в магазине (n < ProductsCount())
std::string ProductDesc(int n);
std::string ProductPrice(int n);
std::string ProductId(int n);
std::string ProductName(const std::string& _id);
std::string ProductDesc(const std::string& _id);
std::string ProductPrice(const std::string& _id);
float ProductNumericPrice(const std::string& _id); // неформатированная цена без валюты
std::string GetCurrencyCode(const std::string& productId);

/// С помощью следующих методов можно узнать, идёт ли процесс покупки.
bool HasActiveTransactions();
bool HasActiveTransactionsForProductId(const std::string& _id);
bool IsBusy(); // Учитывает deferredRestorePurchases, deferredPurchases и HasActiveTransactions.

} // End on namespace

#endif // __cplusplus

#endif // _MICROTRANSACTIONS_H

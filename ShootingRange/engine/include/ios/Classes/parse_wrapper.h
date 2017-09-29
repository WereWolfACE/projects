#ifndef _IOS_PARSE_WRAPPER_H_
#define _IOS_PARSE_WRAPPER_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <json/json.h>

#include "RefCounter.h"

// C++ оболочка для Parse.

namespace ParseWrapper
{

	class Query;
	class Object;
	class Push;
	class Installation;
	typedef boost::intrusive_ptr<Query> QueryPtr;
	typedef boost::intrusive_ptr<Object> ObjectPtr;
	typedef boost::intrusive_ptr<Push> PushPtr;
	typedef boost::intrusive_ptr<Installation> InstallationPtr;
	class QueryImpl;
	class ObjectImpl;
	class PushImpl;

	struct ObjectOperationType {
		enum Type {
			FETCH = 0, // загрузка
			SAVE, // сохранение
			REFRESH, // обновление
			DELETE // удаление с сервера
		};
	};

	struct ValueType {
		enum Type {
			AUTO = 0, // попытаться определить автоматически в get-функции, 
			STRING,
			INT64,
			UINT64,
			DOUBLE,
			BOOL,
			ARRAY,
			DICTIONARY // набор пар ключ-значение (обычный json-объект), не путать с Object, который нужен для связей
		};
	};

	typedef boost::function< void(int error_code, const std::vector<ObjectPtr> &objects) > QueryFindCompleteHandler;
	// уведомление о выполнении запроса объектов
	
	typedef boost::function< void(int error_code, int count) > QueryCountCompleteHandler;
	// уведомление о выполнении запроса подсчета объектов
	
	typedef boost::function< void(int error_code, ObjectOperationType::Type type) > ObjectOperationCompleteHandler;
	// уведомление о завершении асинхронной операции с объектом
	
	typedef boost::function< void(int error_code) > SendPushCompleteHandler;

///////////////////////////////////
// Query
///////////////////////////////////
	class Query
		: public RefCounter
		, private boost::noncopyable
	{
	public:

		static QueryPtr GetSimpleQuery(const std::string &className); // -> +queryWithClassName:
		static QueryPtr GetInstallationQuery(); // -> PFInstallation +query

		virtual ~Query();

		static QueryPtr OrQuery(const std::vector<QueryPtr> &subQueryies); // +orQueryWithSubqueries:
		QueryPtr WhereKeyMatchesKeyInQuery(const std::string &key, const std::string &otherKey, const QueryPtr query); // -> -whereKey:matchesKey:inQuery:
		QueryPtr WhereKeyDoesNotMatchKeyInQuery(const std::string &key, const std::string &otherKey, const QueryPtr query); // -> -whereKey:doesNotMatchKey:inQuery:

		QueryPtr WhereKeyEqualTo(const std::string &key, const ObjectPtr object); // -> -whereKey:equalTo:(PFObject*)
		QueryPtr WhereKeyNotEqualTo(const std::string &key, const ObjectPtr object); // -> -whereKey:notEqualTo:(PFObject*)

		QueryPtr WhereKeyEqualTo(const std::string &key, const Json::Value &value); // -> -whereKey:equalTo:
		QueryPtr WhereKeyNotEqualTo(const std::string &key, const Json::Value &value); // -> -whereKey:notEqualTo:
		QueryPtr WhereKeyLessThanOrEqualTo(const std::string &key, const Json::Value &value); // -> -whereKey:lessThanOrEqualTo:
		QueryPtr WhereKeyLessThan(const std::string &key, const Json::Value &value); // -> -whereKey:lessThan:
		QueryPtr WhereKeyGreaterThanOrEqualTo(const std::string &key, const Json::Value &value); // -> -whereKey:greaterThanOrEqualTo:
		QueryPtr WhereKeyGreaterThan(const std::string &key, const Json::Value &value); // -> -whereKey:greaterThan:
		QueryPtr WhereKeyContainedIn(const std::string &key, const Json::Value &includeSet); // -> -whereKey:containedIn:
		QueryPtr WhereKeyNotContainedIn(const std::string &key, const Json::Value &excludeSet); // -> -whereKey:notContainedIn:
		QueryPtr WhereKeyHasPrefix(const std::string &key, const std::string &prefix); // -> -whereKey:hasPrefix:
		QueryPtr WhereKeyExists(const std::string &key); // -> -whereKeyExists:
		QueryPtr WhereKeyDoesNotExist(const std::string &key); // -> -whereKeyDoesNotExist:

		QueryPtr WhereKeyLessThanOrEqualTo(const std::string &key, time_t value); // -> -whereKey:lessThanOrEqualTo:(NSDate*)
		QueryPtr WhereKeyGreaterThanOrEqualTo(const std::string &key, time_t value); // -> -whereKey:greaterThanOrEqualTo:(NSDate*)

		QueryPtr OrderByAscending(const std::string &key); // -> -orderByAscending:
		QueryPtr OrderByDescending(const std::string &key); // -> -orderByDescending:
		QueryPtr AddAscendingOrder(const std::string &key); // -> -addAscendingOrder:
		QueryPtr AddDescendingOrder(const std::string &key); // -> -addDescendingOrder:

		QueryPtr SelectKeys(const std::vector<std::string> &keys); // -> -selectKeys:
		QueryPtr IncludeKey(const std::string &key); // -> -includeKey:

		QueryPtr SetLimit(int limit); // -> property limit
		QueryPtr SetSkip(int skip); // -> property skip

		void FindObjects(QueryFindCompleteHandler onComplete); // -> –findObjectsInBackgroundWithBlock:
		void CountObjects(QueryCountCompleteHandler onComplete); // -> -countObjectsInBackgroundWithBlock:

		// синхронные методы для тестовых целей
		void FindObjectsBlocking(QueryFindCompleteHandler onComplete); // -> –findObjects:
		void CountObjectsBlocking(QueryCountCompleteHandler onComplete); // -> -countObjects:

		void Cancel();

	private:

		friend class Push;

		std::unique_ptr<QueryImpl> impl; // -> PFQuery

		Query(QueryImpl *impl);
	};


///////////////////////////////////
// Object
///////////////////////////////////
	class Object
		: public RefCounter
		, private boost::noncopyable
	{
	public:

		static ObjectPtr CreateObject(const std::string &className); // -> +objectWithClassName:
		static ObjectPtr CreateObjectWithoutData(const std::string &className, const std::string &objectId); // -> +objectWithoutDataWithClassName:
		virtual ~Object();

		std::string GetObjectId(); // -> property objectId
		time_t GetCreatedAt(); // -> property createdAt
		time_t GetUpdateAt(); // -> property updatedAt

 		void Delete(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -deleteInBackgroundWithBlock:
		void DeleteEventually(); // -> -deleteEventually
		void Fetch(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -fetchInBackgroundWithBlock:
		void Refresh(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -refreshInBackgroundWithBlock:
		void Save(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -saveInBackgroundWithBlock:
		void SaveEventually(); // -> -saveEventually

		static void DeleteAll(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +deleteAllInBackground:block:
		static void FetchAll(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +fetchAllInBackground:block:
		static void SaveAll(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +saveAllInBackground:block:

		// синхронные методы для тестовых целей
		void DeleteBlocking(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -delete:
		void FetchBlocking(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -fetch:
		void RefreshBlocking(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -refresh:
		void SaveBlocking(ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> -save:

		static void DeleteAllBlocking(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +deleteAll:error:
		static void FetchAllBlocking(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +fetchAll:error:
		static void SaveAllBlocking(const std::vector<ObjectPtr> &objects, ObjectOperationCompleteHandler onComplete = ObjectOperationCompleteHandler()); // -> +saveAll:error:

		bool IsDataAvailable(); // -> -isDataAvailable
		bool IsDirty(); // -> -isDirty

		bool ContainsKey(const std::string &key); // -> via -objectForKey:
		std::vector<std::string> AllKeys(); // -> -allKeys

		int GetInt(const std::string &key, int def = 0); // -> -objectForKey:
		void SetInt(const std::string &key, int value); // -> -setObject:forKey:
		int64_t GetInt64(const std::string &key, int64_t def = 0); // -> -objectForKey:
		void SetInt64(const std::string &key, int64_t value); // -> -setObject:forKey:
		double GetDouble(const std::string &key, double def = 0.0); // -> -objectForKey:
		void SetDouble(const std::string &key, double value); // -> -setObject:forKey:
		std::string GetString(const std::string &key, const std::string &def = ""); // -> -objectForKey:
		void SetString(const std::string &key, const std::string &value); // -> -setObject:forKey:

		Json::Value GetValue(const std::string &key, ValueType::Type expectedType = ValueType::AUTO);
			// -> -objectForKey:  *** для простых типов  *** если нет значения, возвращается nullValue
		void SetValue(const std::string &key, const Json::Value &value); // -> -setObject:forKey:  *** для простых типов
		ObjectPtr GetLinkedObject(const std::string &key); // -> -objectForKey:  *** связь один ко многим с другими таблицами
		void SetLinkedObject(const std::string &key, const ObjectPtr obj); // -> -setObject:forKey:  *** связь один ко многим с другими таблицами
		bool GetBytes(const std::string &key, std::vector<uint8_t> &bytes); // -> -objectForKey:  *** NSData  *** если нет значения, возвращается false и bytes не изменяется
		void SetBytes(const std::string &key, const std::vector<uint8_t> &bytes); // -> -setObject:forKey:  *** NSData
		void RemoveKey(const std::string &key); // -> -removeObjectForKey:  *** для простых типов и объектов-связей

		void AddValueToArray(const std::string &key, const Json::Value &value); // -> -addObject:forKey:
		void AddUniqueValueToArray(const std::string &key, const Json::Value &value); // -> -addUniqueObject:forKey:
		void AddAllToArray(const std::string &key, const Json::Value &value); // -> -addObjectsFromArray:forKey:  *** value должно быть массивом
		void AddAllUniqueToArray(const std::string &key, const Json::Value &value); // -> -addUniqueObjectsFromArray:forKey:  *** value должно быть массивом
		void RemoveValueFromArray(const std::string &key, const Json::Value &value); // -> -removeObject:forKey:
		void RemoveValuesFromArray(const std::string &key, const Json::Value &value); // -> -removeObjectsInArray:forKey:  *** value должно быть массивом

	protected:

		friend class Query;
		friend class QueryImpl;
		friend class ObjectImpl;

		std::unique_ptr<ObjectImpl> impl; // -> PFObject

		Object(ObjectImpl *impl);
	};
	
///////////////////////////////////
// Call function
// В документации parse написано, что серверная функция может возращать любые данные (конвертится в json)
// и даже может возращать ParseObject. В нашей обёртке возращается только json.
///////////////////////////////////
	typedef boost::function< void(const Json::Value &result, int error_code) > CallFunctionCompleteHandler;
	void callFunction(const std::string &functionName, const Json::Value &params, CallFunctionCompleteHandler completeHandler);


///////////////////////////////////
// Push
///////////////////////////////////
#if !defined(ENGINE_TARGET_MACOS)
	class Push
		: public RefCounter
		, private boost::noncopyable
	{
	public:

#ifdef __OBJC__
		static void StoreDeviceToken(NSData *token); // -> +storeDeviceToken:
		static void HandlePush(NSDictionary *userInfo);
#endif
		static PushPtr CreatePush();
		virtual ~Push();

		PushPtr ExpireAfter(int seconds); // -> -expireAfterTimeInterval:
		PushPtr ExpireAtDate(time_t date); // -> -expireAtDate:
		PushPtr ClearExpiration(); // -> -clearExpiration

		PushPtr SetData(const Json::Value &data); // -> -setData:
		PushPtr SetQuery(const QueryPtr query); // -> -setQuery:  *** только для запросов, полученных из GetInstallationQuery

		void SendPush(SendPushCompleteHandler onComplete = SendPushCompleteHandler()); // -> -sendPushInBackgroundWithBlock:

		// синхронные методы для тестовых целей
		void SendPushBlocking(SendPushCompleteHandler onComplete = SendPushCompleteHandler()); // -> -sendPush:

	private:

		std::unique_ptr<PushImpl> impl; // -> PFPush

		Push(PushImpl *impl);
	};
#endif

///////////////////////////////////
// Installation
///////////////////////////////////
#if !defined(ENGINE_TARGET_MACOS)
	class Installation
		: public Object
	{
	public:

		static InstallationPtr Current(); // -> +currentInstallation
		std::string GetInstallationId(); // -> property installationId

	private:

		Installation(ObjectImpl *impl);
	};
#endif


///////////////////////////////////
// Functions
///////////////////////////////////
	void Init(const std::string &appId, const std::string &clientKey); // -> +setApplicationId:clientKey:
	
	inline int ErrorCode_NoError() { // Значение "Ошибки нет"
		return 0; // см. PFConstants. Минимальный код ошибки в Parse равен 1
	};
	int ErrorCode_ObjectNotFound(); // Код ошибки "ObjectNotFound"
	int ErrorCode_ConnectionFailed(); // Код ошибки "ConnectionFailed"

}

#endif /* _IOS_PARSE_WRAPPER_H_ */

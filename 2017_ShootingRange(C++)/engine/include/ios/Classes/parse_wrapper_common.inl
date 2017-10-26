#ifndef _PARSE_WRAPPER_COMMON_INL_
#define _PARSE_WRAPPER_COMMON_INL_

namespace ParseWrapper
{

///////////////////////////////////
// Query
///////////////////////////////////
Query::Query(QueryImpl *impl_)
	: impl(impl_)
{}

Query::~Query()
{}

QueryPtr Query::WhereKeyMatchesKeyInQuery(const std::string &key, const std::string &otherKey, const QueryPtr query)
{
	Assert(impl.get());
	Assert(query);
	impl->WhereKeyMatchesKeyInQuery(key, otherKey, query->impl.get());
	return this;
}

QueryPtr Query::WhereKeyDoesNotMatchKeyInQuery(const std::string &key, const std::string &otherKey, const QueryPtr query)
{
	Assert(impl.get());
	Assert(query);
	impl->WhereKeyDoesNotMatchKeyInQuery(key, otherKey, query->impl.get());
	return this;
}

QueryPtr Query::WhereKeyEqualTo(const std::string &key, const ObjectPtr object)
{
	Assert(impl.get());
	Assert(object);
	impl->WhereKeyEqualTo(key, object->impl.get());
	return this;
}

QueryPtr Query::WhereKeyNotEqualTo(const std::string &key, const ObjectPtr object)
{
	Assert(impl.get());
	Assert(object);
	impl->WhereKeyNotEqualTo(key, object->impl.get());
	return this;
}

QueryPtr Query::WhereKeyEqualTo(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyEqualTo(key, value);
	return this;
}

QueryPtr Query::WhereKeyNotEqualTo(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyNotEqualTo(key, value);
	return this;
}

QueryPtr Query::WhereKeyLessThanOrEqualTo(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyLessThanOrEqualTo(key, value);
	return this;
}

QueryPtr Query::WhereKeyLessThan(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyLessThan(key, value);
	return this;
}

QueryPtr Query::WhereKeyGreaterThanOrEqualTo(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyGreaterThanOrEqualTo(key, value);
	return this;
}

QueryPtr Query::WhereKeyGreaterThan(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->WhereKeyGreaterThan(key, value);
	return this;
}

QueryPtr Query::WhereKeyContainedIn(const std::string &key, const Json::Value &includeSet)
{
	Assert(impl.get());
	Assert(includeSet.isArray());
	impl->WhereKeyContainedIn(key, includeSet);
	return this;
}

QueryPtr Query::WhereKeyNotContainedIn(const std::string &key, const Json::Value &excludeSet)
{
	Assert(impl.get());
	Assert(excludeSet.isArray());
	impl->WhereKeyNotContainedIn(key, excludeSet);
	return this;
}

QueryPtr Query::WhereKeyExists(const std::string &key)
{
	Assert(impl.get());
	impl->WhereKeyExists(key);
	return this;
}

QueryPtr Query::WhereKeyHasPrefix(const std::string &key, const std::string &prefix)
{
	Assert(impl.get());
	impl->WhereKeyHasPrefix(key, prefix);
	return this;
}

QueryPtr Query::WhereKeyDoesNotExist(const std::string &key)
{
	Assert(impl.get());
	impl->WhereKeyDoesNotExist(key);
	return this;
}

QueryPtr Query::WhereKeyLessThanOrEqualTo(const std::string &key, time_t value)
{
	Assert(impl.get());
	Assert(key == "createdAt" || key == "updatedAt"); // пока поле с датой вручную создать нельзя
	impl->WhereKeyLessThanOrEqualTo(key, value);
	return this;
}

QueryPtr Query::WhereKeyGreaterThanOrEqualTo(const std::string &key, time_t value)
{
	Assert(impl.get());
	Assert(key == "createdAt" || key == "updatedAt"); // пока поле с датой вручную создать нельзя
	impl->WhereKeyGreaterThanOrEqualTo(key, value);
	return this;
}

QueryPtr Query::OrderByAscending(const std::string &key)
{
	Assert(impl.get());
	impl->OrderByAscending(key);
	return this;
}

QueryPtr Query::OrderByDescending(const std::string &key)
{
	Assert(impl.get());
	impl->OrderByDescending(key);
	return this;
}

QueryPtr Query::AddAscendingOrder(const std::string &key)
{
	Assert(impl.get());
	impl->AddAscendingOrder(key);
	return this;
}

QueryPtr Query::AddDescendingOrder(const std::string &key)
{
	Assert(impl.get());
	impl->AddDescendingOrder(key);
	return this;
}

QueryPtr Query::SelectKeys(const std::vector<std::string> &keys)
{
	Assert(impl.get());
	impl->SelectKeys(keys);
	return this;
}

QueryPtr Query::IncludeKey(const std::string &key)
{
	Assert(impl.get());
	impl->IncludeKey(key);
	return this;
}

QueryPtr Query::SetLimit(int limit)
{
	Assert(impl.get());
	impl->SetLimit(limit);
	return this;
}

QueryPtr Query::SetSkip(int skip)
{
	Assert(impl.get());
	impl->SetSkip(skip);
	return this;
}

void Query::FindObjects(QueryFindCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->FindObjects(onComplete);
}

void Query::CountObjects(QueryCountCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->CountObjects(onComplete);
}

void Query::FindObjectsBlocking(QueryFindCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->FindObjectsBlocking(onComplete);
}

void Query::CountObjectsBlocking(QueryCountCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->CountObjectsBlocking(onComplete);
}

void Query::Cancel()
{
	Assert(impl.get());
	impl->Cancel();
}

///////////////////////////////////
// Object
///////////////////////////////////
Object::Object(ObjectImpl *impl_)
	: impl(impl_)
{}

Object::~Object()
{}

std::string Object::GetObjectId()
{
	Assert(impl.get());
	return impl->GetObjectId();
}

time_t Object::GetCreatedAt()
{
	Assert(impl.get());
	return impl->GetCreatedAt();
}

time_t Object::GetUpdateAt()
{
	Assert(impl.get());
	return impl->GetUpdateAt();
}

void Object::Delete(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->Delete(onComplete);
}

void Object::DeleteEventually()
{
	Assert(impl.get());
	impl->DeleteEventually();
}

void Object::Fetch(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->Fetch(onComplete);
}

void Object::Refresh(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->Refresh(onComplete);
}

void Object::Save(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->Save(onComplete);
}

void Object::SaveEventually()
{
	Assert(impl.get());
	impl->SaveEventually();
}

void Object::DeleteBlocking(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->DeleteBlocking(onComplete);
}

void Object::FetchBlocking(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->FetchBlocking(onComplete);
}

void Object::RefreshBlocking(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->RefreshBlocking(onComplete);
}

void Object::SaveBlocking(ObjectOperationCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->SaveBlocking(onComplete);
}

bool Object::IsDataAvailable()
{
	Assert(impl.get());
	return impl->IsDataAvailable();
}

bool Object::IsDirty()
{
	Assert(impl.get());
	return impl->IsDirty();
}

bool Object::ContainsKey(const std::string &key)
{
	Assert(impl.get());
	return impl->ContainsKey(key);
}

std::vector<std::string> Object::AllKeys()
{
	Assert(impl.get());
	return impl->AllKeys();
}

int Object::GetInt(const std::string &key, int def)
{
	Assert(impl.get());
	return impl->GetInt(key, def);
}

void Object::SetInt(const std::string &key, int value)
{
	Assert(impl.get());
	impl->SetInt(key, value);
}

int64_t Object::GetInt64(const std::string &key, int64_t def)
{
	Assert(impl.get());
	return impl->GetInt64(key, def);
}

void Object::SetInt64(const std::string &key, int64_t value)
{
	Assert(impl.get());
	impl->SetInt64(key, value);
}

double Object::GetDouble(const std::string &key, double def)
{
	Assert(impl.get());
	return impl->GetDouble(key, def);
}

void Object::SetDouble(const std::string &key, double value)
{
	Assert(impl.get());
	impl->SetDouble(key, value);
}

std::string Object::GetString(const std::string &key, const std::string &def)
{
	Assert(impl.get());
	return impl->GetString(key, def);
}

void Object::SetString(const std::string &key, const std::string &value)
{
	Assert(impl.get());
	impl->SetString(key, value);
}

Json::Value Object::GetValue(const std::string &key, ValueType::Type expectedType)
{
	Assert(impl.get());
	return impl->GetValue(key, expectedType);
}

void Object::SetValue(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->SetValue(key, value);
}

void Object::SetLinkedObject(const std::string &key, const ObjectPtr obj)
{
	Assert(impl.get());
	Assert(obj);
	impl->SetLinkedObject(key, obj->impl.get());
}

bool Object::GetBytes(const std::string &key, std::vector<uint8_t> &bytes)
{
	Assert(impl.get());
	return impl->GetBytes(key, bytes);
}

void Object::SetBytes(const std::string &key, const std::vector<uint8_t> &bytes)
{
	Assert(impl.get());
	impl->SetBytes(key, bytes);
}

void Object::RemoveKey(const std::string &key)
{
	Assert(impl.get());
	impl->RemoveKey(key);
}

void Object::AddValueToArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->AddValueToArray(key, value);
}

void Object::AddUniqueValueToArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->AddUniqueValueToArray(key, value);
}

void Object::AddAllToArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	Assert(value.isArray());
	impl->AddAllToArray(key, value);
}

void Object::AddAllUniqueToArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	Assert(value.isArray());
	impl->AddAllUniqueToArray(key, value);
}

void Object::RemoveValueFromArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	impl->RemoveValueFromArray(key, value);
}

void Object::RemoveValuesFromArray(const std::string &key, const Json::Value &value)
{
	Assert(impl.get());
	Assert(value.isArray());
	impl->RemoveValuesFromArray(key, value);
}


///////////////////////////////////
// Push
///////////////////////////////////
#if !defined(ENGINE_TARGET_MACOS)

Push::Push(PushImpl *impl_)
	: impl(impl_)
{}

Push::~Push()
{}

PushPtr Push::ExpireAfter(int seconds)
{
	Assert(impl.get());
	impl->ExpireAfter(seconds);
	return this;
}

PushPtr Push::ExpireAtDate(time_t date)
{
	Assert(impl.get());
	impl->ExpireAtDate(date);
	return this;
}

PushPtr Push::ClearExpiration()
{
	Assert(impl.get());
	impl->ClearExpiration();
	return this;
}

PushPtr Push::SetData(const Json::Value &data)
{
	Assert(impl.get());
	if (!data.isObject()) {
		Assert(false);
		return this;
	}
	impl->SetData(data);
	return this;
}

PushPtr Push::SetQuery(const QueryPtr query)
{
	Assert(impl.get());
	Assert(query);
	impl->SetQuery(query->impl.get());
	return this;
}

void Push::SendPush(SendPushCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->SendPush(onComplete);
}

void Push::SendPushBlocking(SendPushCompleteHandler onComplete)
{
	Assert(impl.get());
	impl->SendPushBlocking(onComplete);
}

#endif // !defined(ENGINE_TARGET_MACOS)

///////////////////////////////////
// Installation
///////////////////////////////////
#if !defined(ENGINE_TARGET_MACOS)

Installation::Installation(ObjectImpl *impl_)
	: Object(impl_)
{}

std::string Installation::GetInstallationId()
{
	Assert(impl.get());
	InstallationImpl *inst = (InstallationImpl*)impl.get();
	return inst->GetInstallationId();
}

#endif // !defined(ENGINE_TARGET_MACOS)

}

#endif /* _PARSE_WRAPPER_COMMON_INL_ */

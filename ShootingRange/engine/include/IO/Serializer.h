#pragma once

#include "Utils/Xml.h"
#include "IO/BinaryReader.h"
#include "IO/BinaryWriter.h"
#include "IO/SerializedContent.h"

namespace IO
{
//--------------------------------------------------------------------------------------------------
// сериализаторы
// в классе создаётся один шаблонный метод, обычно template<class Serializer> void Serialize(Serializer &serializer, int version);
// в него передаётся один из сериализаторов и номер версии
// функции сериализации выглядят как serializer.template Add("имя_данных", ссылка_на_данные);
// кроме стандартной Add, есть специализированные методы, со своим набором параметров:
// AddEnum - для перечислений
// AddObject - для объектов, имеющих свой сериализатор
// AddVector - для векторов сериализуемых объектов
// AddPtrVector - для векторов сериализуемых объектов, заданных указателями
// AddScaled, AddScaledInt - для хранения отмасштабированных значений
// Для бинарных сериализаторов:
// AddUInt32 - для хранения ровно 4-х байт без упаковки
// GetOffset/SetOffset - для получения или перемещения на заданную позицию от начала потока
// Для xml-сериализаторов:
// AddAttribute - для создания атрибута, чтобы потом, возможно, занести в него значение
// для специализации потока сериализуемых данных есть методы IsReader/IsWriter и IsXml, IsEmpty
// в сериализаторе есть поле content для записи в оглавление

//--------------------------------------------------------------------------------------------------
// части для сборки сериализаторов

//--------------------------------------------------------------------------------------------------
template<class Base>
class WriterSerializerMixin : public Base
{
public:
	bool IsReader() const { return false; }
	bool IsWriter() const { return true; }

	void AddScaled(const char *attribute, const float &value, float scale)
	{
		float v = value * scale;
		Base::Add(attribute, v);
	}

	void AddScaledInt(const char *attribute, const float &value, float scale)
	{
		int v = int(value * scale);
		Base::Add(attribute, v);
	}

public:
	SerializedContent content;
};

template<class Base>
class ReaderSerializerMixin : public Base
{
public:
	bool IsReader() const { return true; }
	bool IsWriter() const { return false; }

	void AddScaled(const char *attribute, float &value, float scale)
	{
		Base::Add(attribute, value);
		value *= 1.0f / scale;
	}

	void AddScaledInt(const char *attribute, float &value, float scale)
	{
		int v;
		Base::Add(attribute, v);
		value = v / scale;
	}

public:
	SerializedContent content;
};

//--------------------------------------------------------------------------------------------------
class XmlSerializerMixin : public boost::noncopyable
{
public:
	bool IsXml() const { return true; }
	bool IsEmpty() const { return !_elem; }

	void AddUInt32(const char *attribute, uint32_t &value) { Assert(false); }
	size_t GetOffset() { Assert(false); return 0; }
	void SetOffset(size_t offset) { Assert(false); }

protected:
	rapidxml::xml_node<>* _elem = nullptr;
};

class BinarySerializerMixin : public boost::noncopyable
{
public:
	bool IsXml() const { return false; }

	void AddAttribute(const char *attribute, const char *value) { Assert(false); }
};

//--------------------------------------------------------------------------------------------------
template<class Base>
class IfSerializerMixin : public Base
{
public:
	void If(bool condition, const char *description, std::function<void()> doFn, std::function<void()> elseFn = std::function<void()>())
	{
		if (condition)
		{
			if (doFn) doFn();
		}
		else
		{
			if (elseFn) elseFn();
		}
	}
};

//--------------------------------------------------------------------------------------------------
template<class Base, typename ObjectType>
class ReferencesSerializerMixin : public Base
{
protected:
	static const uint32_t NullReferenceId = 0xFFFFFFFF;

	template<typename T>
	void AddObjectImpl(const T &object, int id)
	{
		if (id < 0)
		{
			Assert(false);
			return;
		}
		while(_objects.size() <= (size_t)id)
			_objects.push_back(std::make_pair((void *)nullptr, std::type_index(typeid(void))));
		_objects[id] = std::make_pair((void *)&object, std::type_index(typeid(T)));
	}

	uint32_t FindObject(ObjectType pointer)
	{
		for (size_t i = 0; i < _objects.size(); ++i)
		{
			if (_objects[i].first == pointer)
			{
				return (uint32_t)i;
			}
		}
		return NullReferenceId;
	}

	std::vector<std::pair<ObjectType, std::type_index>> _objects;
};

template<class Base, typename ObjectType>
class ReferencesReaderSerializerMixin : public ReferencesSerializerMixin<Base, ObjectType>
{
protected:
	using ReferencesSerializerMixin<Base, ObjectType>::NullReferenceId;
	using ReferencesSerializerMixin<Base, ObjectType>::_objects;

	struct Reference
	{
		uint32_t id = NullReferenceId;
		std::type_index type = std::type_index(typeid(void));
		std::function<void(ObjectType)> setter;
	};
public:
	void ResolveReferences()
	{
		for (const auto &reference : _references)
		{
			if ((reference.id >= (uint32_t)_objects.size()) || !_objects[reference.id].first || (reference.type != _objects[reference.id].second))
			{
				Assert(false);
				continue;
			}
			reference.setter(_objects[reference.id].first);
		}
		_references.clear();
		_objects.clear();
	}

protected:
	template<typename T>
	void AddReferenceImpl(uint32_t id, T &value)
	{
		if (id == NullReferenceId)
		{
			value = nullptr;
			return;
		}

		Reference reference;
		reference.id = id;
		reference.type = std::type_index(typeid(typename T::element_type));
		reference.setter = [&value](void *pointer)
		{
			value = reinterpret_cast<typename T::element_type *>(pointer);
		};
		_references.push_back(reference);
	}

protected:
	std::vector<Reference> _references;
};

//--------------------------------------------------------------------------------------------------
class XmlWriterSerializerMixin : public ReferencesSerializerMixin<IfSerializerMixin<XmlSerializerMixin>, const void *>
{
private:
	struct Reference
	{
		rapidxml::xml_node<>* element = nullptr;
		std::string attribute;
		const void *pointer = nullptr;
	};
public:
	template<typename T> void Add(const char *attribute, const T &value);

	template<typename T>
	void AddEnum(const char *attribute, const T &value, const std::vector<std::pair<T, const char *>> &reflection)
	{
		std::string v;
		for (auto i : reflection)
		{
			if (value == i.first)
			{
				v = i.second;
				break;
			}
		}
		Add(attribute, v);
	}

	void AddAttribute(const char *attribute, const char *value) { Xml::SetStringAttribute(_elem, attribute, value); }

	template<typename T, class Serializer>
	void AddObject(const char *objectName, const char *nodeName, const char *nameAttribute, T &value, Serializer &serializer, int version)
	{
		rapidxml::xml_node<>* root = _elem;
		_elem = Xml::NewNode(root, nodeName);
		Add<std::string>(nameAttribute, objectName);
		value.Serialize(serializer, version);
		_elem = root;
	}

	template<typename T>
	void AddReference(const char *attribute, T &value)
	{
		if (value.get())
		{
			Reference reference;
			reference.element = _elem;
			reference.attribute = attribute;
			reference.pointer = value.get();
			_references.push_back(reference);
		}
		else
		{
			Xml::SetIntAttribute(_elem, attribute, -1);
		}
	}

	template<typename T>
	void AddObject(const char *attribute, const T &object)
	{
		int id = (int)_objects.size();
		Xml::SetIntAttribute(_elem, attribute, id);
		AddObjectImpl(object, id);
	}

	void ResolveReferences()
	{
		for (const auto &reference : _references)
		{
			uint32_t id = FindObject(reference.pointer);
			if (id == NullReferenceId)
			{
				Assert(false);
				continue;
			}
			Xml::SetIntAttribute(reference.element, reference.attribute, id);
		}
		_references.clear();
		_objects.clear();
	}

private:
	std::vector<Reference> _references;
};

template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const bool &value) { Xml::SetBoolAttribute(_elem, attribute, value); }
template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const float &value) { Xml::SetFloatAttribute(_elem, attribute, value); }
template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const int &value) { Xml::SetIntAttribute(_elem, attribute, value); }
template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const short &value) { Xml::SetIntAttribute(_elem, attribute, value); }
template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const uint8_t &value) { Xml::SetIntAttribute(_elem, attribute, value); }
template<> inline void XmlWriterSerializerMixin::Add(const char *attribute, const std::string &value) { Xml::SetStringAttribute(_elem, attribute, value); }

class XmlReaderSerializerMixin : public ReferencesReaderSerializerMixin<IfSerializerMixin<XmlSerializerMixin>, void *>
{
public:
	template<typename T>
	void Add(const char *attribute, T &value) { Xml::QueryAttribute(_elem, attribute, value);	}

	template<typename T>
	void AddEnum(const char *attribute, T &value, const std::vector<std::pair<T, const char *>> &reflection)
	{
		std::string v;
		Add(attribute, v);
		for (auto i : reflection)
		{
			if (v == i.second)
			{
				value = i.first;
				return;
			}
		}
	}

	void AddAttribute(const char *attribute, const char *value) { Assert(false); }

	template<typename T, class Serializer>
	void AddObject(const char *objectName, const char *nodeName, const char *nameAttribute, T &value, Serializer &serializer, int version)
	{
		rapidxml::xml_node<>* root = _elem;
		_elem = FindNodeByName(root, objectName, nodeName, nameAttribute);
		value.Serialize(serializer, version);
		_elem = root;
	}

	template<typename T>
	void AddReference(const char *attribute, T &value)
	{
		int id = NullReferenceId;
		Add<int>(attribute, id);
		AddReferenceImpl((uint32_t)id, value);
	}

	template<typename T>
	void AddObject(const char *attribute, T &object)
	{
		int id = NullReferenceId;
		Add<int>(attribute, id);
		if (id == NullReferenceId) return;
		AddObjectImpl(object, id);
	}

private:
	static rapidxml::xml_node<>* FindNodeByName(rapidxml::xml_node<>* root, const std::string &name, const char *nodeName, const char *nameAttribute)
	{
		rapidxml::xml_node<>* paramElem = root->first_node(nodeName);
		while (paramElem) {
			if (name == paramElem->first_attribute(nameAttribute)->value()) {
				return paramElem;
			}
			paramElem = paramElem->next_sibling(nodeName);
		}
		return NULL;
	}
};

//--------------------------------------------------------------------------------------------------
class BinaryWriterSerializerMixin : public ReferencesSerializerMixin<IfSerializerMixin<BinarySerializerMixin>, const void *>
{
private:
	struct Reference
	{
		size_t offset;
		const void *pointer;
	};
public:
	bool IsEmpty() const { return !_writer; }

	template<typename T> void Add(const char *attribute, const T &value);

	template<typename T, class Serializer>
	void AddObject(const char *objectName, const char *nodeName, const char *nameAttribute, T &value, Serializer &serializer, int version)
	{
		Add<std::string>(nameAttribute, objectName);
		value.Serialize(serializer, version);
	}

	void AddUInt32(const char *attribute, const uint32_t &value) { _writer->WriteUInt32(value); }
	void AddData(const char *attribute, const void *data, size_t size) { _writer->GetBaseStream()->Write(data, size); }
	size_t GetOffset() { return _writer->GetBaseStream()->Tell(); }
	void SetOffset(size_t offset) { _writer->GetBaseStream()->Seek((int)offset, IO::Origin::Beginning); }

	template<typename T>
	void AddReference(const char *attribute, const T &value)
	{
		if (value.get())
		{
			Reference reference;
			reference.offset = GetOffset();
			reference.pointer = value.get();
			_references.push_back(reference);
		}
		uint32_t tempReference = NullReferenceId;
		AddUInt32("", tempReference);
	}

	template<typename T>
	void AddObject(const char *attribute, const T &object)
	{
		AddObjectImpl(object, (int)_objects.size());
	}

	void ResolveReferences()
	{
		if (!_references.empty())
		{
			auto restoreOffset = GetOffset();
			for (const auto &reference : _references)
			{
				uint32_t id = FindObject(reference.pointer);
				if (id == NullReferenceId)
				{
					Assert(false);
					continue;
				}
				SetOffset(reference.offset);
				AddUInt32("", id);
			}
			SetOffset(restoreOffset);
		}
		_references.clear();
		_objects.clear();
	}

private:
	void WriteInt(int value)
	{
		if (!_compact)
		{
			_writer->WriteInt32(value);
		}
		else
		{
			_writer->Write7BitEncodedInt32(value);
		}
	}

	void WriteBool(bool value)
	{
		if (!_compact)
		{
			_writer->WriteBool(value);
		}
		else
		{
			if ((_bitShift == 0) || (_bitShift == 8) || ((_bitsOffset + sizeof(uint8_t)) != GetOffset()))
			{
				_bitsOffset = GetOffset();
				_bitShift = 0;
				_bits = 0;
			}
			else
			{
				SetOffset(_bitsOffset);
			}

			if (value) _bits |= 1 << _bitShift;
			_writer->WriteUInt8(_bits);
			++_bitShift;
		}
	}

protected:
	IO::BinaryWriter *_writer = nullptr;
	bool _compact = false;

private:
	uint8_t _bits = 0;
	uint8_t _bitShift = 0;
	size_t _bitsOffset = 0;

	std::vector<Reference> _references;
};

template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const bool &value) { WriteBool(value); }
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const float &value) { _writer->WriteFloat(value); }
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const int &value) { WriteInt(value); }
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const uint32_t &value) { WriteInt(value); }
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const short &value) { WriteInt(value & 0xFFFF); }
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const uint8_t &value)
{
	if (!_compact)
	{
		_writer->WriteInt32(value);
	}
	else
	{
		_writer->WriteUInt8(value);
	}
}
template<> inline void BinaryWriterSerializerMixin::Add(const char *attribute, const std::string &value) { _writer->WriteUtf8String(value); }

class BinaryReaderSerializerMixin : public ReferencesReaderSerializerMixin<IfSerializerMixin<BinarySerializerMixin>, void *>
{
public:
	bool IsEmpty() const { return !_reader; }

	template<typename T> void Add(const char *attribute, T &value);

	template<typename T, class Serializer>
	void AddObject(const char *objectName, const char *nodeName, const char *nameAttribute, T &value, Serializer &serializer, int version)
	{
		std::string name;
		Add<std::string>(nameAttribute, name);
		Assert(name == objectName);
		value.Serialize(serializer, version);
	}

	void AddUInt32(const char *attribute, uint32_t &value) { value = _reader->ReadUInt32(); }
	void AddData(const char *attribute, void *data, size_t size) { _reader->GetBaseStream()->Read(data, size); }
	size_t GetOffset() { return _reader->GetBaseStream()->Tell(); }
	void SetOffset(size_t offset) { _reader->GetBaseStream()->Seek((int)offset, IO::Origin::Beginning); }

	template<typename T>
	void AddReference(const char *attribute, T &value)
	{
		uint32_t id = NullReferenceId;
		AddUInt32("", id);
		AddReferenceImpl(id, value);
	}

	template<typename T>
	void AddObject(const char *attribute, T &object)
	{
		AddObjectImpl(object, (int)_objects.size());
	}

private:
	int ReadInt()
	{
		if (!_compact)
		{
			return _reader->ReadInt32();
		}
		else
		{
			return _reader->Read7BitEncodedInt32();
		}
	}

	bool ReadBool()
	{
		if (!_compact)
		{
			return _reader->ReadBool();
		}
		else
		{
			if ((_bitShift == 0) || (_bitShift == 8) || ((_bitsOffset + sizeof(uint8_t)) != GetOffset()))
			{
				_bitsOffset = GetOffset();
				_bitShift = 0;
				_bits = _reader->ReadUInt8();
			}

			bool result = (_bits & (1 << _bitShift)) != 0;
			++_bitShift;
			return result;
		}
	}

protected:
	IO::BinaryReader *_reader = nullptr;
	bool _compact = false;

private:
	uint8_t _bits = 0;
	uint8_t _bitShift = 0;
	size_t _bitsOffset = 0;
};

template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, bool &value) { value = ReadBool(); }
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, float &value) { value = _reader->ReadFloat(); }
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, int &value) { value = ReadInt(); }
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, uint32_t &value) { value = (uint32_t)ReadInt(); }
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, short &value) { value = (short)ReadInt(); }
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, uint8_t &value)
{
	value = _compact ? _reader->ReadUInt8() : (uint8_t)_reader->ReadInt32();
}
template<> inline void BinaryReaderSerializerMixin::Add(const char *attribute, std::string &value) { value = _reader->ReadUtf8String(); }

//--------------------------------------------------------------------------------------------------
template<class Base>
class BinaryEnumWriterSerializerMixin : public Base
{
public:
	template<typename T>
	void AddEnum(const char *attribute, const T &value, const std::vector<std::pair<T, const char *>> &reflection)
	{
		int v = (int)value;
		Base::Add(attribute, v);
	}
};

template<class Base>
class BinaryEnumReaderSerializerMixin : public Base
{
public:
	template<typename T>
	void AddEnum(const char *attribute, T &value, const std::vector<std::pair<T, const char *>> &reflection)
	{
		int v = 0;
		Base::Add(attribute, v);
		for (auto i : reflection)
		{
			if (v == (int)i.first)
			{
				value = i.first;
				break;
			}
		}
	}
};

//--------------------------------------------------------------------------------------------------
template<class Base>
class BinaryVectorWriterSerializerMixin : public Base
{
public:
	template<typename T, class Serializer, typename... Args>
	void AddPtrVector(const char *nodeName, const std::vector<boost::intrusive_ptr<T>> &value, Serializer &serializer, int version, std::function<T*()> constructor, Args... args)
	{
		int size = (int)value.size();
		Base::Add("", size);
		for (int i = 0; i < size; ++i)
		{
			value[i]->Serialize(serializer, version, args...);
		}
	}

	template<typename T, class Serializer, typename... Args>
	void AddVector(const char *nodeName, std::vector<T> &value, Serializer &serializer, int version, std::function<T()> constructor, Args... args)
	{
		int size = (int)value.size();
		Base::Add("", size);
		for (int i = 0; i < size; ++i)
		{
			value[i].Serialize(serializer, version, args...);
		}
	}
};

template<class Base>
class BinaryVectorReaderSerializerMixin : public Base
{
public:
	template<typename T, class Serializer, typename... Args>
	void AddPtrVector(const char *nodeName, std::vector<boost::intrusive_ptr<T>> &value, Serializer &serializer, int version, std::function<T*()> constructor, Args... args)
	{
		int size = 0;
		Base::Add("", size);
		Assert(size >= 0);
		value.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			value.push_back(constructor());
			value.back()->Serialize(serializer, version, args...);
		}
	}

	template<typename T, class Serializer, typename... Args>
	void AddVector(const char *nodeName, std::vector<T> &value, Serializer &serializer, int version, std::function<T()> constructor, Args... args)
	{
		int size = 0;
		Base::Add("", size);
		Assert(size >= 0);
		value.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			value.push_back(constructor());
			value.back().Serialize(serializer, version, args...);
		}
	}
};

//--------------------------------------------------------------------------------------------------
template<class Base>
class XmlVectorWriterSerializerMixin : public Base
{
public:
	using Base::_elem;

	template<typename T, class Serializer, typename... Args>
	void AddPtrVector(const char *nodeName, const std::vector<boost::intrusive_ptr<T>> &value, Serializer &serializer, int version, std::function<T*()> constructor, Args... args)
	{
		rapidxml::xml_node<>* root = _elem;
		for (auto &i : value)
		{
			_elem = Xml::NewNode(root, nodeName);
			i->Serialize(serializer, version, args...);
		}
		_elem = root;
	}

	template<typename T, class Serializer, typename... Args>
	void AddVector(const char *nodeName, std::vector<T> &value, Serializer &serializer, int version, std::function<T()> constructor, Args... args)
	{
		rapidxml::xml_node<>* root = _elem;
		for (auto &i : value)
		{
			_elem = Xml::NewNode(root, nodeName);
			i.Serialize(serializer, version, args...);
		}
		_elem = root;
	}
};

template<class Base>
class XmlVectorReaderSerializerMixin : public Base
{
public:
	using Base::_elem;

	template<typename T, class Serializer, typename... Args>
	void AddPtrVector(const char *nodeName, std::vector<boost::intrusive_ptr<T>> &value, Serializer &serializer, int version, std::function<T*()> constructor, Args... args)
	{
		value.reserve(CountVectorSize(nodeName, _elem));
		rapidxml::xml_node<>* root = _elem;
		_elem = root->first_node(nodeName);
		while (_elem)
		{
			value.push_back(constructor());
			value.back()->Serialize(serializer, version, args...);
			_elem = _elem->next_sibling(nodeName);
		}
		_elem = root;
	}

	template<typename T, class Serializer, typename... Args>
	void AddVector(const char *nodeName, std::vector<T> &value, Serializer &serializer, int version, std::function<T()> constructor, Args... args)
	{
		value.reserve(CountVectorSize(nodeName, _elem));
		rapidxml::xml_node<>* root = _elem;
		_elem = root->first_node(nodeName);
		while (_elem)
		{
			value.push_back(constructor());
			value.back().Serialize(serializer, version, args...);
			_elem = _elem->next_sibling(nodeName);
		}
		_elem = root;
	}

private:
	static size_t CountVectorSize(const char *nodeName, rapidxml::xml_node<> *root)
	{
		size_t count = 0;
		for (rapidxml::xml_node<> *node = root->first_node(nodeName); node; node = node->next_sibling(nodeName))
		{
			++count;
		}
		return count;
	}
};

//--------------------------------------------------------------------------------------------------
// сериализатор для записи в xml-файл
class XmlWriterSerializer : public XmlVectorWriterSerializerMixin<WriterSerializerMixin<XmlWriterSerializerMixin>>
{
public:
	explicit XmlWriterSerializer(rapidxml::xml_node<>* elem) { _elem = elem; }
};

// сериализатор для чтения xml-файла
class XmlReaderSerializer : public XmlVectorReaderSerializerMixin<ReaderSerializerMixin<XmlReaderSerializerMixin>>
{
public:
	explicit XmlReaderSerializer(rapidxml::xml_node<>* elem) { _elem = elem; }
};

// сериализатор для записи бинарного файла, параметр compact=true включает упаковку данных (7bit-encoded для целых, и т.п.)
class BinaryWriterSerializer : public BinaryVectorWriterSerializerMixin<BinaryEnumWriterSerializerMixin<WriterSerializerMixin<BinaryWriterSerializerMixin>>>
{
public:
	BinaryWriterSerializer(IO::BinaryWriter *writer, bool compact) { _writer = writer; _compact = compact; }
};

// сериализатор для чтения бинарного файла, параметр compact=true включает упаковку данных (7bit-encoded для целых, и т.п.)
class BinaryReaderSerializer : public BinaryVectorReaderSerializerMixin<BinaryEnumReaderSerializerMixin<ReaderSerializerMixin<BinaryReaderSerializerMixin>>>
{
public:
	BinaryReaderSerializer(IO::BinaryReader *reader, bool compact) { _reader = reader; _compact = compact; }
};

//--------------------------------------------------------------------------------------------------
class BinaryFormatSerializerMixin : public BinarySerializerMixin
{
public:
	bool IsEmpty() const { return false; }

	template<typename T> void Add(const char *attribute, const T &value);

	template<typename T, class Serializer, typename... Args>
	void AddPtrVector(const char *nodeName, const std::vector<boost::intrusive_ptr<T>> &value, Serializer &serializer, int version, std::function<T*()> constructor, Args... args)
	{
		AddFormat(nodeName, "pvector");
	}

	template<typename T, class Serializer, typename... Args>
	void AddVector(const char *nodeName, const std::vector<T> &value, Serializer &serializer, int version, std::function<T()> constructor, Args... args)
	{
		AddFormat(nodeName, "vector");
	}

	template<typename T, class Serializer>
	void AddObject(const char *objectName, const char *nodeName, const char *nameAttribute, T &value, Serializer &serializer, int version)
	{
		if (_skipMode) return;
		_stream << "object:" << nodeName << "(" << nameAttribute << "=" << objectName << ");";
	}

	void AddUInt32(const char *attribute, const uint32_t &value) { AddFormat(attribute, "uint32"); }
	void AddData(const char *attribute, const void *data, size_t size) { AddFormat(attribute, "data"); }
	size_t GetOffset() { return (size_t)_stream.tellp(); }
	void SetOffset(size_t offset) { _skipMode = (offset != (size_t)_stream.tellp()); }

	template<typename T>
	void AddReference(const char *attribute, const T &value)
	{
		AddFormat(attribute, "reference");
	}

	template<typename T>
	void AddObject(const char *attribute, const T &object)
	{
	}

	void ResolveReferences()
	{
	}

	std::string GetFormat() const { return _stream.str(); }

	void If(bool condition, const char *description, std::function<void()>doFn, std::function<void()>elseFn = std::function<void()>())
	{
		if (_skipMode) return;
		_stream << "if(" << description << "){";
		if (doFn) doFn();
		_stream << "}else{";
		if (elseFn) elseFn();
		_stream << "};";
	}
private:
	void AddFormat(const char *attribute, const char *type)
	{
		if (_skipMode) return;
		_stream << type << ":" << attribute << ";";
	}

private:
	std::stringstream _stream;
	bool _skipMode = false;
};

template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const bool &value) { AddFormat(attribute, "bool"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const float &value) { AddFormat(attribute, "float"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const int &value) { AddFormat(attribute, "int"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const uint32_t &value) { AddFormat(attribute, "uint"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const short &value) { AddFormat(attribute, "short"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const uint8_t &value) { AddFormat(attribute, "byte"); }
template<> inline void BinaryFormatSerializerMixin::Add(const char *attribute, const std::string &value) { AddFormat(attribute, "string"); }

//--------------------------------------------------------------------------------------------------
// вспомогательный сериализатор, создаёт строку формата сериализуемых бинарных данных
class BinaryFormatSerializer : public BinaryEnumReaderSerializerMixin<WriterSerializerMixin<BinaryFormatSerializerMixin>>
{
public:
	BinaryFormatSerializer() {}
};

}

// инстанцирование всех вариантов стандартного сериализатора класса
#define CLASS_SERIALIZATION(macClassName)\
template void macClassName::Serialize<IO::XmlWriterSerializer>(IO::XmlWriterSerializer &serializer, int version);\
template void macClassName::Serialize<IO::XmlReaderSerializer>(IO::XmlReaderSerializer &serializer, int version);\
template void macClassName::Serialize<IO::BinaryWriterSerializer>(IO::BinaryWriterSerializer &serializer, int version);\
template void macClassName::Serialize<IO::BinaryReaderSerializer>(IO::BinaryReaderSerializer &serializer, int version);\
template void macClassName::Serialize<IO::BinaryFormatSerializer>(IO::BinaryFormatSerializer &serializer, int version);

#define CLASS_SERIALIZATION_EXTENDED(macClassName, ...)\
template void macClassName::Serialize<IO::XmlWriterSerializer>(IO::XmlWriterSerializer &serializer, int version, __VA_ARGS__);\
template void macClassName::Serialize<IO::XmlReaderSerializer>(IO::XmlReaderSerializer &serializer, int version, __VA_ARGS__);\
template void macClassName::Serialize<IO::BinaryWriterSerializer>(IO::BinaryWriterSerializer &serializer, int version, __VA_ARGS__);\
template void macClassName::Serialize<IO::BinaryReaderSerializer>(IO::BinaryReaderSerializer &serializer, int version, __VA_ARGS__);\
template void macClassName::Serialize<IO::BinaryFormatSerializer>(IO::BinaryFormatSerializer &serializer, int version, __VA_ARGS__);

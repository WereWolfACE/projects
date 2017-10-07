#pragma once

namespace IO
{
//--------------------------------------------------------------------------------------------------
// кусок сериализуемых бинарных данных
// в начале куска хранится длина данных (4 байта) и тип куска (4 байта), затем сами данные
template<class Serializer>
class ChunkSerializer : public boost::noncopyable
{
public:
	// конструктор для чтения куска
	explicit ChunkSerializer(Serializer &serializer) :
		_serializer(serializer)
	{
		if (_serializer.IsXml() || _serializer.IsWriter())
		{
			Assert(false);
			return;
		}
		Read();
	}

	// конструктор для записи или проверки типа куска при чтении
	ChunkSerializer(uint32_t fourcc, Serializer &serializer, bool skip) :
		_serializer(serializer)
		, _fourcc(fourcc)
		, _skip(skip)
	{
		if (_skip) return;
		if (_serializer.IsXml()) return;

		if (_serializer.IsWriter())
		{
			uint32_t size = 0;
			_offset = _serializer.GetOffset();
			_serializer.AddUInt32("size", size);
			_serializer.AddUInt32("fourcc", fourcc);
		}
		else
		{
			Read();
			Assert(fourcc == _fourcc);
		}
	}

	// деструктор, должен вызываться в момент окончания сериализации куска
	// при записи записывает длину куска, при чтении проверяет, что сериализатор дочитал кусок до конца,
	~ChunkSerializer()
	{
		if (_skip) return;
		if (_serializer.IsXml()) return;

		if (_serializer.IsWriter())
		{
			size_t pos = _serializer.GetOffset();
			_serializer.SetOffset(_offset);
			uint32_t size = (uint32_t)(pos - (_offset + size_t(2 * sizeof(uint32_t))));
			_serializer.AddUInt32("size", size);
			_serializer.SetOffset(pos);
		}
		else
		{
			Assert(_offset == _serializer.GetOffset());
		}
	}

	// пропустить кусок при чтении
	void Skip()
	{
		if (_skip) return;
		if (_serializer.IsXml()) return;
		if (_serializer.IsWriter()) return;
		_serializer.SetOffset(_offset);
	}

	// тип куска
	uint32_t GetFourCC() const
	{
		return _fourcc;
	}

	// смещение куска от начала потока
	size_t GetOffset() const
	{
		return _offset;
	}

private:
	void Read()
	{
		uint32_t size = 0;
		_serializer.AddUInt32("size", size);
		_serializer.AddUInt32("fourcc", _fourcc);
		_offset = _serializer.GetOffset() + size;
	}

private:
	bool _skip = false;
	size_t _offset = 0;
	uint32_t _fourcc = 0;
	Serializer &_serializer;
};

}

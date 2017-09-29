#pragma once

#include "IO/ChunkSerializer.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | \
	((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))
#endif

namespace IO
{

// оглавление файла данных
// хранится в конце файла, сначала кусок данных (тип dataFourcc) потом оглавление (тип contentFourcc)
// внутри куска данных хранятся куски на которые ссылается оглавление
class SerializedContent
{
public:
	// запись оглавления
	struct Entry
	{
		// смещение данных от начала
		uint32_t offset = 0;
		// тип данных
		uint32_t fourcc = 0;
		// идентификатор данных
		std::string name;

		Entry()
		{
		}

		Entry(uint32_t entryOffset, uint32_t entryFourcc, const std::string &entryName) :
			offset(entryOffset)
			, fourcc(entryFourcc)
			, name(entryName)
		{
		}

		template<class Serializer>
		Entry(ChunkSerializer<Serializer> &chunk, const std::string &entryName) :
			name(entryName)
		{
			offset = (uint32_t)chunk.GetOffset();
			fourcc = chunk.GetFourCC();
		}

		template<class Serializer>
		void Serialize(Serializer &serializer, int version)
		{
			serializer.AddUInt32("offset", offset);
			serializer.AddUInt32("fourcc", fourcc);
			serializer.Add("name", name);
		}

		operator bool() const
		{
			return fourcc != 0;
		}
	};

	// тип куска данных
	static const uint32_t dataFourcc = MAKEFOURCC('D', 'a', 't', 'a');
	// тип куска оглавления
	static const uint32_t contentFourcc = MAKEFOURCC('C', 'o', 'n', 't');
public:
	template<class Serializer>
	void Serialize(Serializer &serializer, int version)
	{
		IO::ChunkSerializer<Serializer> chunk(contentFourcc, serializer, false);
		serializer.template AddVector<Entry, Serializer>("ContentEntry", _entries, serializer, version, []{ return Entry(); });
	}

	const std::vector<Entry> &GetEntries() const { return _entries; }
	void Add(const Entry &entry) { _entries.push_back(entry); }

private:
	std::vector<Entry> _entries;
};

}

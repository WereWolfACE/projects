#pragma once

#include <cstdint>
#include <string>
#include <ostream>

class Version {
public:
	Version() = default;
	explicit Version(uint32_t major, uint32_t minor = 0, uint32_t revision = 0, uint32_t build = 0);
	explicit Version(const char* str);
	explicit Version(const std::string& str);

	Version(const Version&) = default;
	Version& operator=(const Version&) = default;

	bool operator==(const Version& rhs) const;
	bool operator<(const Version& rhs) const;

	bool operator!=(const Version& rhs) const { return !operator==(rhs); }
	bool operator>=(const Version& rhs) const { return !operator<(rhs); }
	bool operator>(const Version& rhs) const { return !operator==(rhs) && !operator<(rhs); }
	bool operator<=(const Version& rhs) const { return !operator>(rhs); }

	friend std::ostream& operator<<(std::ostream& os, const Version& v);
	friend std::istream& operator>>(std::istream& is, Version& v);

	uint32_t Major() const { return _major; }
	uint32_t Minor() const { return _minor; }
	uint32_t Revision() const { return _revision; }
	uint32_t Build() const { return _build; }

	Version& Major(uint32_t value) { _major = value; return *this; }
	Version& Minor(uint32_t value) { _minor = value; return *this; }
	Version& Revision(uint32_t value) { _revision = value; return *this; }
	Version& Build(uint32_t value) { _build = value; return *this; }

	std::string ToString() const;

private:
	void Parse(const char* str);

private:
	uint32_t _major = 0;
	uint32_t _minor = 0;
	uint32_t _revision = 0;
	uint32_t _build = 0;
};

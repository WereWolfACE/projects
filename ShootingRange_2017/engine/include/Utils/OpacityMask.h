#ifndef __MASK_H__
#define __MASK_H__

#if _MSC_VER > 1200
#pragma once
#endif // _MSC_VER

namespace Render {
	class Image;
}

namespace utils {

class OpacityMask {
public:
	OpacityMask();
	explicit OpacityMask(const Render::Image& image, int alphaLimit);
	explicit OpacityMask(const std::string& filename);
	explicit OpacityMask(const uint8_t* data);

	void Clear();

	void Load(const Render::Image& image, int alphaLimit);
	void Load(const std::string& filename);
	void Load(const uint8_t* data);

	void Save(const std::string& filename) const;

	bool IsEmpty() const { return _rows.empty(); }

	bool HitTest(int destWidth, int destHeight, int x, int y) const;

private:
	int GetWidth() const;
	int GetHeight() const;

private:
	typedef std::pair<uint16_t, uint16_t> Range;
	typedef std::vector<Range> Row;
	typedef std::vector<Row> Rows;
	
	Rows _rows;
	int _width;

private:
	OpacityMask(const OpacityMask&);
	OpacityMask& operator=(const OpacityMask&);
};

} // namespace utils

#endif // __MASK_H__

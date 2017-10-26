#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#pragma once

#include "Render/Texture.h"

namespace Render {

enum ImageType {
	IT_NONE = 0,
	IT_PNG = 1 << 0,
	IT_JPG = 1 << 1,
	IT_WEBP = 1 << 2,
	IT_PVRTC = 1 << 3,
	IT_DDS = 1 << 4
};

class ImageLoader
{
public:
	ImageLoader(Render::Image* image, const std::string& filename);

	void Load();

	static ImageType LoadFileInfo(const std::string& filename, GfxFileInfo *info);

	static void LoadJPG(Image* image, const std::string& file, GfxFileInfo *info = 0);
	static void LoadPNG(Image* image, const std::string& file, GfxFileInfo *info = 0);
	static void LoadWEBP(Image* image, const std::string& file, GfxFileInfo *info = 0);
	static void LoadPVRTC(Image* image, const std::string& file, GfxFileInfo *info = 0);
	static void LoadDDS(Image* image, const std::string& file, GfxFileInfo *info = 0);

	static void LoadJPGWithMask(Image* image, const std::string& file, const std::string& file_a);

	static ImageType SelectImageType(std::string &file, std::string &file_a);
	static ImageType CheckImageTypeBySignature(const std::string& filename);

private:
	friend class Render::Image;

	Image* _image;
	std::string _filename;
	IO::InputStreamPtr _stream;
};

class ResizeToPowerOfTwo
{
public:
	void apply(Image *image);
};

class AddAlphaFromImage
{
	Image *_alpha_source;
public:
	explicit AddAlphaFromImage(Image* alpha_source);

	void apply(Image *image);
};

} // namespace Render

bool SaveImageAsJpeg(Render::Image* image, const std::string &path, int quality);
bool SaveImageAsPng(Render::Image* image, const std::string &path);
bool SaveImageAsBmp(Render::Image* image, const std::string &path);

#endif // #ifndef _IMAGELOADER_H_

#ifndef _GAMEFEATURES_H_
#define _GAMEFEATURES_H_

#include "Render/Texture.h"

bool SaveScreenshot(const std::string &name, Render::Texture **tex, const IRect &rect, const std::string &sigPath = "", const IPoint &sigPos = IPoint(0, 0));
// сохранить скриншот в файл на диске, если name непусто
// сохранить скриншот в текстуру, если tex не NULL
// добавить на скриншот логотип в позицию sigPos, если sigPath не пуст
// возвращает true, если name непусто и файл удачно сохранился
// если текстура создалась, то *tex не NULL, иначе - NULL

#endif

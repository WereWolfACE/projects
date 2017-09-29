#ifndef __PLAYRIXENGINE_H__
#define __PLAYRIXENGINE_H__

/// Этот файл включает все заголовки для PlayrixEngine

#include "types.h"

#include "EngineAssert.h"
#include "Utils.h"
#include "File.h"
#include "Render.h"
#include "Core.h"
#include "EngineIO.h"
#include "GUI.h"
#include "MM.h"
#include "Flash.h"
#include "SceneGraph.h"
#include "DataStore.h"
#include "Distortion.h"
#include "Factory.h"
#include "ImageLoader.h"
#include "Layer.h"
#include "MessageFunc.h"
#include "MiniSlider.h"
#include "Particles.h"
#include "SmoothTextureChanger.h"
#include "Spline.h"
#include "GameFeatures.h"
#include "Marketing.h"
#include "DeviceInfo.h"
#include "InternetReachability.h"
#include "KeyboardManager.h"


#if defined(ENGINE_TARGET_MACOS) || defined(ENGINE_TARGET_IPHONE)
#include "ios/Classes/GCInterface.h"

#include "ios/Classes/MicroTransactions.h"

#elif defined(ENGINE_TARGET_ANDROID)
#include "GCInterface.h"
#include "MicroTransactions.h"
#endif


#endif // __PLAYRIXENGINE_H__

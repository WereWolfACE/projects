#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include <RefCounter.h>

#include "KeyFramedAnimation.h"
#include "ModelNode.h"
#include "BoneContent.h"
#include "MeshDataBase.h"

namespace SceneGraph
{

class BinaryContentProvider
{
public:
	virtual ModelNode* getModel() = 0;
	virtual void ReadMeshSkininfo(uint32_t _skininfoOffset, BonesContent& _skinInfo) = 0;
	virtual void ReadMeshData(uint32_t _meshOffset, MeshDataBase::Vertices* _vertices, MeshDataBase::Indices* _indeces) = 0;
	virtual void ReadAnimationData(uint32_t _animationdataoffset, KeyFramedAnimation* _animation) = 0;

public:
	static BinaryContentProvider* createInstance(const std::string& _filename);
};

typedef boost::shared_ptr<BinaryContentProvider> BinaryContentProviderPtr;

};

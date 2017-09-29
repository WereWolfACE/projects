#ifndef __BONECONTENT_H__
#define __BONECONTENT_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include <vector>
#include <boost/intrusive_ptr.hpp>

#include <Utils/Matrix4.h>
#include <RefCounter.h>

namespace SceneGraph {

///
/// Кость (веса вершин конкретного меша).
///
class CBone : public RefCounter
{
public:
	struct Weight{
		Weight(int _vertexindex, float _boneweight):
			vertexindex(_vertexindex),
			boneweight(_boneweight)
		{

		}

		int vertexindex; 
		float boneweight; 
	};

	typedef std::vector<Weight> Weights;

public:	
	CBone(const std::string& _name, const math::Matrix4& _invMatrix);

	size_t GetMemoryInUse() const;

	inline const std::string& getName(){return __m_name;};		
	inline const math::Matrix4& GetBoneOffset() const{ return __m_invMatrix; };
	
	inline const Weights& GetWeights() const{ return _weights; };
	inline void AddWeight(const Weight& weight){ _weights.push_back(weight); }; //используется толлько в момент создания прототипа

private:
	std::string __m_name; //имя кости, используется для связывания костей с нодами(фреймами) скелета
	Weights _weights;
	math::Matrix4 __m_invMatrix; //обратная матрица перобразования кости, вносит координаты верших из глобальной системы координат в систему координат связаную с костью
};

typedef boost::intrusive_ptr<CBone> CBonePtr;
typedef std::vector<CBonePtr> BonesContent;

} // namespace SceneGraph

#endif // __BONECONTENT_H__

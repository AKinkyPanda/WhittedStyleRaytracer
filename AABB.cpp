#include "precomp.h"
#include "AABB.h"

namespace Tmpl8 
{
	AABB::AABB() 
	{

	}

	AABB::AABB(vec3 min, vec3 max) 
		: min(min), max(max)
	{

	}

	void AABB::CalcCenter()
	{
		this->center = this->min + 0.5f * (this->max - this->min);
	}

	float AABB::CalcSurfArea() const
	{
		const vec3 diagonal = (this->max - this->min).absolute();

		return ((diagonal.x * diagonal.y) + (diagonal.x * diagonal.z) + (diagonal.z * diagonal.y)) * 2;
	}

}
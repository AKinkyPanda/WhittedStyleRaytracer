#include "precomp.h"

namespace Tmpl8
{
	Ray::Ray(const point3& origin, const vec3& direction)
		: O(origin), D(direction.normalized())
	{

	}

	point3 Ray::origin() const
	{
		return O;
	}

	vec3 Ray::direction() const
	{
		return D;
	}

	point3 Ray::at(float t) const
	{
		return O + t * D;
	}
}
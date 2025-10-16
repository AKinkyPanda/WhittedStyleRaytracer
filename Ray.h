#pragma once

namespace Tmpl8 {
	class Ray
	{
		point3 O;
		vec3 D;

	public:
		Ray() = default;
		Ray(const point3& origin, const vec3& direction);

		point3 origin() const;
		vec3 direction() const;
		point3 at(float t) const;
	};
}


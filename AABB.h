#pragma once

namespace Tmpl8 {
	class AABB
	{
	public:
		AABB();
		AABB(vec3 min, vec3 max);

		vec3 min, max, center;

		void CalcCenter();
		float CalcSurfArea() const;
	};
}

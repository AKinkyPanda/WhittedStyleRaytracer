#pragma once

namespace Tmpl8 {
	class Camera
	{
	public:
		point3 E;
		vec3 V;
		float d;

		point3 C;
		vec3 p0;
		vec3 p1;
		vec3 p2;

		vec3 VN;
		vec3 up;
		vec3 right;

		vec3 vertical, horizontal;

		Camera(point3 cameraPos, vec3 viewDir, float focal_length);

		Ray GetRay(float x, float y) const;
	};
}
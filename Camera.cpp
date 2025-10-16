#include "precomp.h"

namespace Tmpl8
{
	Camera::Camera(point3 cameraPos, vec3 viewDir, float focal_length)
		: E(cameraPos), V(viewDir), d(focal_length)
	{
		up = vec3(0, 1, 0);
		right = vec3(1, 0, 0);

		VN = normalize(V - E);
		right = normalize(cross(up, VN));
		up = cross(VN, right);

		C = E + d * VN;

		p0 = C - right + up * aspectRatio;
		p1 = C + right + up * aspectRatio;
		p2 = C - right - up * aspectRatio;

		vertical = p1 - p0;
		horizontal = p2 - p0;
	}

	Ray Camera::GetRay(float x, float y) const
	{
		return Ray(E, p0 + x * vertical + y * horizontal - E);
	}
}
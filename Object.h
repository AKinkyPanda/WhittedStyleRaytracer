#pragma once
#include <memory>

namespace Tmpl8{
	class AABB;
	class Material;

	class HitRecord
	{
	public:
		point3 p;
		vec3 normal;
		color color;
		float specular;
		float reflective;
		float t = INFINITY;
		bool front_face;
		MATERIALTYPE type;
		float ir; // Index of refraction;

		void SetFrontFace(const Ray& r, const vec3& outward_normal);
	};

	class Object
	{
	public:
		std::unique_ptr<Material> mat_ptr;
		std::unique_ptr<AABB> aabb_ptr;

		virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const = 0;
	};

	class Ellipsoid
		: public Object
	{
		point3 center;
		vec3 size;

	public:
		Ellipsoid() = default;
		Ellipsoid(point3 center_, vec3 size_, std::unique_ptr<Material> mat);

		virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
	};
}


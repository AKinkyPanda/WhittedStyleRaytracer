#include "precomp.h"

namespace Tmpl8
{
	Sphere::Sphere(point3 center, float radius, std::unique_ptr<Material> mat)
		: C(center), radius(radius)
	{
		mat_ptr = std::move(mat);
		this->aabb_ptr = std::make_unique<AABB>(this->C - this->radius, this->C + this->radius);
		this->aabb_ptr->CalcCenter();
	}

	bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
	{
		const vec3 oc = r.origin() - C;
		const auto a = r.direction().sqrLentgh();
		const auto half_b = dot(oc, r.direction());
		const auto c = oc.sqrLentgh() - radius * radius;

		const auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		const auto sqrtd = sqrtf(discriminant);

		// Find the nearest root that lies in the acceptable range.
		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || t_max < root) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || t_max < root)
				return false;
		}

		record.t = root;
		record.p = r.at(record.t);
		const vec3 s = record.p - C;
		const float m = 1.f / radius;
		const vec3 outward_normal = s * m;
		record.SetFrontFace(r, outward_normal);
		record.type = mat_ptr->GetMatType();

		if (record.type == DIELECTRIC)
		{
			record.ir = mat_ptr->GetMatVal().x;
			record.color = mat_ptr->GetMatCol();
			record.specular = 0;
			record.reflective = 0;
		}
		else if (record.type == METAL)
		{
			record.color = mat_ptr->GetMatCol();
			record.specular = mat_ptr->GetMatVal().x;
			record.reflective = mat_ptr->GetMatVal().y;
		}
		else
		{
			record.color = mat_ptr->GetMatCol();
			record.specular = 0;
			record.reflective = 0;
		}

		return true;
	}
}

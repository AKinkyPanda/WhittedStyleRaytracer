#include "precomp.h"

namespace Tmpl8
{
	void HitRecord::SetFrontFace(const Ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

	///////////////////////////////
	// Ellipsoid
	///////////////////////////////

	Ellipsoid::Ellipsoid(point3 center_, vec3 size_, std::unique_ptr<Material> mat)
		: center(center_), size(size_)
	{
		this->mat_ptr = std::move(mat);

		this->aabb_ptr = std::make_unique<AABB>(center - size, center + size);
		this->aabb_ptr->CalcCenter();
	}

	bool Ellipsoid::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
	{
		const vec3 oc = r.origin() - center;

		const float a = ((r.direction().x * r.direction().x) / (size.x * size.x))
				+ ((r.direction().y * r.direction().y) / (size.y * size.y))
				+ ((r.direction().z * r.direction().z) / (size.z * size.z));

		const float b = ((2 * oc.x * r.direction().x) / (size.x * size.x))
				+ ((2 * oc.y * r.direction().y) / (size.y * size.y))
				+ ((2 * oc.z * r.direction().z) / (size.z * size.z));
		const float half_b = b / 2;

		const float c = ((oc.x * oc.x) / (size.x * size.x))
				+ ((oc.y * oc.y) / (size.y * size.y))
				+ ((oc.z * oc.z) / (size.z * size.z))
				- 1;

		const float discriminant = half_b * half_b - a * c;
		if (discriminant < 0) 
			return false;

		const float sqrtd = sqrtf(discriminant);

		float root = (-half_b - sqrtd) / a;
		if (root < t_min || t_max < root) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || t_max < root)
				return false;
		}

		record.t = root;
		record.p = r.at(record.t);
		const vec3 s = record.p - center;
		const vec3 m = vec3(1.f / size.x, 1.f / size.y, 1.f / size.z);
		const vec3 outward_normal = normalize(s * m * m);
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
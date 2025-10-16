#include "precomp.h"

namespace Tmpl8
{
	///////////////////////////////////
	/// Diffuse Material
	///	///////////////////////////////

	Diffuse::Diffuse(color color)
		: col(color)
	{
		
	}

	bool Diffuse::Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const
	{
		return true;
	}

	MATERIALTYPE Diffuse::GetMatType() const
	{
		return type;
	}

	vec3 Diffuse::GetMatVal() const
	{
		return vec3(0, 0, 0);
	}

	color Diffuse::GetMatCol() const
	{
		return col;
	}

	///////////////////////////////////
	/// Metal Material
	///	///////////////////////////////

	Metal::Metal(color color, float specularity, float reflectance)
		: col(color), spec(specularity), refl(reflectance)
	{
		
	}

	bool Metal::Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const
	{
		return true;
	}

	MATERIALTYPE Metal::GetMatType() const
	{
		return type;
	}

	vec3 Metal::GetMatVal() const
	{
		return vec3(spec, refl, 0);
	}

	color Metal::GetMatCol() const
	{
		return col;
	}

	///////////////////////////////////
	/// Dielectric Material
	///	///////////////////////////////

	Dielectric::Dielectric(float indexRefraction)
		: ir(indexRefraction)
	{
		
	}

	bool Dielectric::Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const
	{
		return true;
	}

	MATERIALTYPE Dielectric::GetMatType() const
	{
		return type;
	}

	vec3 Dielectric::GetMatVal() const
	{
		return vec3(ir, ir, ir);
	}

	color Dielectric::GetMatCol() const
	{
		return color(0, 0, 0);
	}

}
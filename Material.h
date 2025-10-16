#pragma once
#include "template.h"

namespace Tmpl8 {

	class Material
	{
	public:
		virtual bool Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const = 0;
		virtual MATERIALTYPE GetMatType() const = 0;
		virtual vec3 GetMatVal() const = 0;
		virtual color GetMatCol() const = 0;
	};

	class Diffuse
		: public Material
	{
		color col;
		MATERIALTYPE type = DIFFUSE;

	public:
		Diffuse(color color);

		bool Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const override;
		MATERIALTYPE GetMatType() const override;
		vec3 GetMatVal() const override;
		color GetMatCol() const override;
	};

	class Metal
		: public Material
	{
		color col;
		float spec;
		float refl;
		MATERIALTYPE type = METAL;

	public:
		Metal(color color, float specularity, float reflectance);

		bool Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const override;
		MATERIALTYPE GetMatType() const override;
		vec3 GetMatVal() const override;
		color GetMatCol() const override;
	};

	class Dielectric
		: public Material
	{
		float ir; // Index of refraction
		MATERIALTYPE type = DIELECTRIC;

	public:
		Dielectric(float indexRefraction);

		bool Scatter(const Ray& r_in, const HitRecord& record, color& attenuation, Ray& scattered) const override;
		MATERIALTYPE GetMatType() const override;
		vec3 GetMatVal() const override;
		color GetMatCol() const override;
	};
}

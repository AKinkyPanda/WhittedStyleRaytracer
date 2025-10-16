#pragma once

namespace Tmpl8 {
	enum TYPE { AMBIENT, POINT, DIRECTIONAL };

	class Light
	{
	public:
		virtual TYPE GetType() const = 0;
		virtual float GetIntensity() const = 0;
		virtual vec3 GetDirectionPosition() const = 0;
	};

	class Ambient
		: public Light
	{
		float I; // Intensity
		TYPE type = TYPE::AMBIENT;

	public:
		Ambient(float intensity);

		TYPE GetType() const override;
		float GetIntensity() const override;
		vec3 GetDirectionPosition() const override;
	};

	class Point
		: public Light
	{
		float I;
		point3 position;
		TYPE type = TYPE::POINT;


	public:
		Point(float intensity, point3 pos);

		TYPE GetType() const override;
		float GetIntensity() const override;
		vec3 GetDirectionPosition() const override;
	};

	class Directional
		: public Light
	{
		float I;
		vec3 direction;
		TYPE type = TYPE::DIRECTIONAL;


	public:
		Directional(float intensity, vec3 dir);

		TYPE GetType() const override;
		float GetIntensity() const override;
		vec3 GetDirectionPosition() const override;
	};
}

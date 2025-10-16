#include "precomp.h"

namespace Tmpl8
{
	/* Ambient Light */

	Ambient::Ambient(float intensity)
		: I(intensity)
	{
		
	}

	TYPE Ambient::GetType() const
	{
		return type;
	}

	float Ambient::GetIntensity() const
	{
		return I;
	}

	vec3 Ambient::GetDirectionPosition() const
	{
		return I;
	}

	/* Point Light */

	Point::Point(float intensity, point3 pos)
		: I(intensity), position((pos))
	{
		
	}

	TYPE Point::GetType() const
	{
		return type;
	}

	float Point::GetIntensity() const
	{
		return I;
	}

	vec3 Point::GetDirectionPosition() const
	{
		return position;
	}

	/* Directional Light */

	Directional::Directional(float intensity, vec3 dir)
		: I(intensity), direction(dir)
	{
		
	}

	TYPE Directional::GetType() const
	{
		return type;
	}

	float Directional::GetIntensity() const
	{
		return I;
	}

	vec3 Directional::GetDirectionPosition() const
	{
		return direction;
	}

}
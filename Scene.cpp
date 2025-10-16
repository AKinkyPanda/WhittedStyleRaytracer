#include "precomp.h"

namespace Tmpl8
{
	Scene::Scene()
	{
	}

	Scene::Scene(std::unique_ptr<Object> object)
	{
		Add(std::move(object));
	}

	Scene::Scene(std::unique_ptr<Light> light)
	{
		Add(std::move(light));
	}

	void Scene::Clear()
	{
		objects.clear();
		lights.clear();
		delete node;
		delete objectIndices;
	}

	void Scene::Add(std::unique_ptr<Object> object)
	{
		objects.push_back(std::move(object));
		//bvh->ConstructBVH(this->objects,this->node, this->objectIndices);
	}

	void Scene::Add(std::unique_ptr<Light> light)
	{
		lights.push_back(std::move(light));
	}

	void Scene::BuildBVH()
	{
		bvh->ConstructBVH(this->objects, this->node, this->objectIndices);
	}

	bool Scene::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const
	{
		if (bvhState == true)
		{

			bool hitIs = false;

			if (!bvh->Traverse(r, record, this->node, objects, t_min, t_max, hitIs, objectIndices))
				return false;

			const point3 P = r.at(record.t);
			const vec3 N = record.normal;
			float I = ComputeLighting(P, N, -r.direction(), record.specular);
			record.color = record.color * I;

			return hitIs;
		}
		else
		{
			HitRecord temp_record;
			bool is_hit = false;
			auto closest_so_far = t_max;

			for (const auto& object : objects)
			{
				if (object->Hit(r, t_min, closest_so_far, temp_record))
				{
					is_hit = true;
					closest_so_far = temp_record.t;
					record = temp_record;

					const point3 P = r.at(record.t);
					const vec3 N = record.normal;
					float I = ComputeLighting(P, N, -r.direction(), record.specular);
					record.color = record.color * I;
				}
			}

			return is_hit;
		}
	}

	float Scene::ComputeLighting(point3 P, vec3 N, vec3 V, float s) const
	{
		float I = 0.f; // Intensity
		vec3 L;
		float t_max;

		for(const auto& light : lights)
		{
			if(light->GetType() == AMBIENT)
			{
				I += light->GetIntensity();
			}
			else
			{
				if (light->GetType() == POINT)
				{
					L = light->GetDirectionPosition() - P;
					t_max = 1.f;
				}
				else
				{
					L = light->GetDirectionPosition();
					t_max = INFINITY;
				}

				// Shadow Ray
				HitRecord shadow_record;
				Ray shadow_r = Ray(P, L);
				if (Hit(shadow_r, 0.001f, t_max - 2 * 0.001f, shadow_record))
				{
					continue;
				}

				// Lambertian reflectance - "matte"
				const auto n_dot_l = dot(N, L);

				if (n_dot_l > 0)
				{
					I += light->GetIntensity() * n_dot_l / (N.length() * L.length());
				}

				// Specular reflection - "mirror"
				if (s > 0)
				{
					const auto R = 2 * N * n_dot_l - L;
					const auto r_dot_v = dot(R, V);

					if ( r_dot_v > 0)
					{
						I += light->GetIntensity() * powf(r_dot_v / (R.length() * V.length()), s);
					}
				}
			}
		}

		return I;
	}
}
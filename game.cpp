#include "precomp.h"
#include "game.h"

#include <ostream>
#include <windows.h>

#include "Sphere.h"
#include "surface.h"
#include <algorithm>
#include <execution>

namespace Tmpl8
{

	float inverseWidth;
	float inverseHeight;
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		if (firstInit)
		{
			printf("Controls:\n");
			printf("- Arrows for camera movement.\n");
			printf("- WASD for camera rotation.\n");
			printf("- Q & E for camera movement forwards and backwards.\n");
			printf("- F1-F4 for changing scenes.\n");
			printf("- B for toggling bvh on and off.\n");
			firstInit = false;
		}
		uint count = 0;

		auto matte = std::make_unique<Diffuse>(color(0, 1, 0));
		auto metal = std::make_unique<Metal>(color(1, 0, 0), 0.5f, 0.4f);
		auto glass = std::make_unique<Dielectric>(1.5f);
		auto matte1 = std::make_unique<Diffuse>(color(0, 0, 1));

		switch(sceneSet)
		{
		case DEFAULT:
			scene_.Clear();

			cam = Camera(point3(0, 0, 0), vec3(0, 0, 1), 1);

			scene_.Add(std::make_unique<Sphere>(point3(0, -202.f, 0), 200, std::move(matte)));

			scene_.Add(std::make_unique<Sphere>(point3(0, 0, 4), 1, std::move(glass)));
			scene_.Add(std::make_unique<Sphere>(point3(4, 0, 10), 1, std::move(metal)));

			scene_.Add(std::make_unique<Ellipsoid>(point3(-2.f, 0.f, 10.f), vec3(2.f, 1.f, 1.f), std::move(matte1)));

			break;

		case SPHERES256:
			scene_.Clear();

			cam = Camera(point3(0, 0, 0), vec3(0, 0, 1), 1);

			// 256 Primitives
			count = 0;
			for (int i = -10; i <= 2; i += 2)
			{
				for (int j = -15; j < 3; j += 3)
				{
					for (int k = 40; k <= 50; k += 2)
					{
						matte = std::make_unique<Diffuse>(color(0, 1, 0));
						scene_.Add(std::make_unique<Sphere>(point3((float)i, (float)j, (float)k), 1, std::move(matte)));
						count++;
					}
				}
			}

			count += 4;
			matte = std::make_unique<Diffuse>(color(0, 1, 0));
			scene_.Add(std::make_unique<Sphere>(point3(-12, -15, 40), 1, std::move(matte)));
			matte = std::make_unique<Diffuse>(color(0, 1, 0));
			scene_.Add(std::make_unique<Sphere>(point3(-12, -12, 40), 1, std::move(matte)));
			matte = std::make_unique<Diffuse>(color(0, 1, 0));
			scene_.Add(std::make_unique<Sphere>(point3(-12, -9, 40), 1, std::move(matte)));
			matte = std::make_unique<Diffuse>(color(0, 1, 0));
			scene_.Add(std::make_unique<Sphere>(point3(-12, -6, 40), 1, std::move(matte)));

			printf("There are %i spheres.\n", count);
			break;

		case SPHERES800:
			scene_.Clear();

			cam = Camera(point3(0, 0, 0), vec3(0, 0, 1), 1);

			// 800 Primitives
			count = 0;
			for (int i = -10; i <= 20; i += 2)
			{
				for (int j = -15; j < 15; j += 3)
				{
					for (int k = 40; k < 50; k += 2)
					{
						matte = std::make_unique<Diffuse>(color(0, 1, 0));
						scene_.Add(std::make_unique<Sphere>(point3((float)i, (float)j, (float)k), 1, std::move(matte)));
						count++;
					}
				}
			}
			printf("There are %i spheres.\n", count);
			break;

		case SPHERES16:
			scene_.Clear();

			cam = Camera(point3(0, 0, 0), vec3(0, 0, 1), 1);

			// 16 000 Primitives
			count = 0;
			for (int i = -26; i < 26; i += 2)
			{
				for (int j = -33; j < 33; j += 3)
				{
					for (int k = 40; k < 96; k += 2)
					{
						matte = std::make_unique<Diffuse>(color(0, 1, 0));
						scene_.Add(std::make_unique<Sphere>(point3((float)i, (float)j, (float)k), 1, std::move(matte)));
						count++;
					}
				}
			}

			printf("There are %i spheres.\n", count);
		}


		scene_.Add(std::make_unique<Point>(0.8f, point3(4, 10, 10)));

		//scene_.Add(std::make_unique<Ambient>(0.2f));
		//scene_.Add(std::make_unique<Point>(0.6f, point3(0, 3, 2)));
		//scene_.Add(std::make_unique<Directional>(0.2f, vec3(-2, 1, 0)));

		m_horizontalIter.resize(ScreenWidth);
		m_verticalIter.resize(ScreenHeight);

		for (uint32_t i = 0; i < ScreenWidth; i++)
			m_horizontalIter[i] = i;		
		for (uint32_t i = 0; i < ScreenHeight; i++)
			m_verticalIter[i] = i;

		inverseWidth = 1.f / (ScreenWidth - 1.f);
		inverseHeight = 1.f / (ScreenHeight - 1.f);

		scene_.BuildBVH();
	}
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		timer_.reset();
		// clear the graphics window
		screen->Clear(0);

		HandleInput();

#define MT 1
#if MT
		std::for_each(std::execution::par, m_horizontalIter.begin(), m_horizontalIter.end(),
			[this](uint32_t x)
			{
				std::for_each(std::execution::par, m_verticalIter.begin(), m_verticalIter.end(),
					[this, x](uint32_t y)
					{
						float i = (float)x * inverseWidth;
						float j = (float)y * inverseHeight;

						Ray r = cam.GetRay(i, j);
						const color pixel_color = TraceRay(r, scene_, 0, INFINITY, max_depth);
						PlotColor(screen, pixel_color, x, y);
					});
			});
#else
		for (int x = 0; x < ScreenWidth; x++)
		{
			for (int y = 0; y < ScreenHeight; y++)
			{
				float i = (float)x / (ScreenWidth - 1.f);
				float j = (float)y / (ScreenHeight - 1.f);

				Ray r = cam.GetRay(i, j);
				const color pixel_color = TraceRay(r, scene_, 0, INFINITY, max_depth);
				PlotColor(screen, pixel_color, x, y);
			}
		}
#endif

		char buffer[20];
		sprintf(buffer, "FPS: %f\n", 1000.f / timer_.elapsed());
		screen->Print(buffer, 2, 2, 0xFFFFFF);
	}

	color Game::TraceRay(const Ray& r, const Object& scene, float t_min, float t_max, int recursion_depth) const
	{
		HitRecord record;
		vec3 test;

		if (!scene.Hit(r, t_min, t_max, record))
			//return (1.f - t) * color(1.f, 1.f, 1.f) + t * color(0.5f, 0.7f, 1.f);
			return color(0.5f, 0.7f, 1.f);

		if (record.type == METAL && recursion_depth > 0 && record.reflective > 0)
		{
			const auto R = test.ReflectRay(-r.direction(), record.normal);
			const auto reflected_color = TraceRay(Ray(r.at(record.t), R), scene, 0.001f, INFINITY, recursion_depth - 1);
			return record.color * (1 - record.reflective) + reflected_color * record.reflective;
		}
		else if (record.type == DIELECTRIC && recursion_depth > 0)
		{
			const float cos_theta = fmin(record.normal.dot(-r.direction()), 1.f);
			const float refraction_ratio = record.front_face ? (1.f / record.ir) : record.ir;
			const float f = Fresnel(cos_theta, refraction_ratio);
			return f * TraceRay(Ray(record.p, test.ReflectRay(-r.direction(), record.normal)), scene, 0.001f, INFINITY, recursion_depth - 1)
				+ (1 - f) * TraceRay(Ray(record.p, test.RefractRay(r.direction(), record.normal, refraction_ratio)), scene, 0.001f, INFINITY, recursion_depth - 1);
		}
		else
		{
			return record.color;
		}

		//return 0.5f * (record.normal + color(1, 1, 1));
	}

	void Game::PlotColor(Surface* a_Target, color pixel_color, int x, int y)
	{
		const float r = pixel_color.x;
		const float g = pixel_color.y;
		const float b = pixel_color.z;

		const int ir = static_cast<int>(256 * Clamp(r, 0.0f, 0.999f));
		const int ig = static_cast<int>(256 * Clamp(g, 0.0f, 0.999f));
		const int ib = static_cast<int>(256 * Clamp(b, 0.0f, 0.999f));

		//a_Target->Plot(x, y, ir * 256 * 256 + ig * 256 + ib);
		a_Target->Plot(x, y, (ir << 16) + (ig << 8) + ib);
	}

	float Game::Fresnel(float cos_theta, float refraction_ratio) const
	{
		// Use Schlick's approximation for reflectance
		auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cos_theta), 5.f);
	}

	void Game::HandleInput()
	{
		point3 cameraSpeed = 0.2f;

		// move the camera
		if (GetAsyncKeyState(VK_DOWN))
		{
			cam.E -= cam.up * cameraSpeed;
			cam.V -= cam.up * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			cam.E += cam.right * cameraSpeed;
			cam.V += cam.right * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState(VK_UP))
		{
			cam.E += cam.up * cameraSpeed;
			cam.V += cam.up * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			cam.E -= cam.right * cameraSpeed;
			cam.V -= cam.right * cameraSpeed;
			scene_.camChange = true;
		}

		// rotate the camera
		if (GetAsyncKeyState('A'))
		{
			cam.V -= cam.right * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState('D'))
		{
			cam.V += cam.right * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState('W'))
		{
			cam.V += cam.up * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState('S'))
		{
			cam.V -= cam.up * cameraSpeed;
			scene_.camChange = true;
		}

		if (GetAsyncKeyState('Q'))
		{
			cam.E += cam.VN * cameraSpeed;
			cam.V += cam.VN * cameraSpeed;
			scene_.camChange = true;
		}
		if (GetAsyncKeyState('E'))
		{
			cam.E -= cam.VN * cameraSpeed;
			cam.V -= cam.VN * cameraSpeed;
			scene_.camChange = true;
		}

		if (GetAsyncKeyState(VK_F1))
		{
			scene_.camChange = true;
			sceneSet = DEFAULT;
			Init();
		}
		if (GetAsyncKeyState(VK_F2))
		{
			scene_.camChange = true;
			sceneSet = SPHERES256;
			Init();
		}
		if (GetAsyncKeyState(VK_F3))
		{
			scene_.camChange = true;
			sceneSet = SPHERES800;
			Init();
		}
		if (GetAsyncKeyState(VK_F4))
		{
			scene_.camChange = true;
			sceneSet = SPHERES16;
			Init();
		}

		if (GetAsyncKeyState('B'))
		{
			if (scene_.bvhState == true)
			{
				scene_.bvhState = false;
			}
			else
			{
				scene_.bvhState = true;
			}
		}

		if (scene_.camChange == true)
		{
			cam = Camera(cam.E, cam.V, cam.d);
			scene_.camChange = false;
		}
	}

};
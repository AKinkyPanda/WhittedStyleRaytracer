#pragma once

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
	float Fresnel(float cos_theta, float refraction_ratio) const;
	color TraceRay(const Ray& r, const Object& scene_, float t_min, float t_max, int recursion_depth) const;
	void PlotColor(Surface* a_Target, color pixel_color, int x, int y);
	void HandleInput();

private:
	Surface* screen{ nullptr };

	Camera cam = Camera(point3(0, 0, 0), vec3(0, 0, 1), 1);
	Scene scene_;
	timer timer_;
	enum Scene {DEFAULT, SPHERES256, SPHERES800, SPHERES16};
	Scene sceneSet = DEFAULT;
	bool firstInit = true;

	std::vector<uint32_t> m_horizontalIter, m_verticalIter;
	int max_depth = 3;
};

}; // namespace Tmpl8
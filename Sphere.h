#pragma once

namespace Tmpl8 {
    class Sphere :
        public Object
    {
        point3 C;
        float radius;

    public:
        Sphere() = default;
        Sphere(point3 center, float radius, std::unique_ptr<Material> mat);

        virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;
    };
}
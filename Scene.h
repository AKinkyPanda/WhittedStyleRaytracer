#pragma once
#include <memory>
#include <vector>

#include "BVH.h"

namespace Tmpl8 {
	class Light;

	class Scene :
        public Object
    {
        std::vector<std::unique_ptr<Object>> objects;
        std::vector<std::unique_ptr<Light>> lights;
        std::unique_ptr<BVH> bvh;
        BVHNode* node;
        uint* objectIndices;

    public:
        Scene();
        Scene(std::unique_ptr<Object> object);
        Scene(std::unique_ptr<Light> light);

        bool camChange = false;
        bool bvhState = true;

        void Clear();
        void Add(std::unique_ptr<Object> object);
        void Add(std::unique_ptr<Light> light);
        void BuildBVH();

        virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const override;

        float ComputeLighting(point3 P, vec3 N, vec3 V, float s) const;
    };
}

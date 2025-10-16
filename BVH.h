#pragma once
#include "AABB.h"

namespace Tmpl8 {

	class BVHNode 
	{
	public:
		BVHNode();
		~BVHNode();

		AABB bounds;
		bool isLeaf;
		BVHNode *left, *right;
		uint first, count;

		bool intersect(vec3 O, vec3 invD, HitRecord& record);
		void destroy();
	};

	class BVH
	{
	public:
		BVH();


		void ConstructBVH(const std::vector<std::unique_ptr<Object>> &objects, BVHNode* &node, uint* &objectIndices);

		void CalcBounds(const std::vector<std::unique_ptr<Object>> &objects, BVHNode* &root, uint* &indices);
		void Subdivide(const std::vector<std::unique_ptr<Object>> &objects, BVHNode* &root, uint* &indices);
		void Partition(const std::vector<std::unique_ptr<Object>> &objects, BVHNode* &root, uint binCount, uint* &indices);

		bool Traverse(const Ray& r, HitRecord& record, BVHNode* node, const std::vector<std::unique_ptr<Object>>& objects, float t_min, float t_max, bool &hitIs, uint* objectIndices);
	};
}


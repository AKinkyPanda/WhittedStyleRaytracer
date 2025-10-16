#include "precomp.h"

#include <algorithm>

constexpr uint MAX_PRIMITIVES = 2;
constexpr uint BINS_COUNT = 10;

namespace Tmpl8
{
	///////////////////
	///BVHNode
	/////////////////////

	BVHNode::BVHNode()
	{
		this->isLeaf = false;
	}

	BVHNode::~BVHNode()
	{
		//this->destroy();
	}


	bool BVHNode::intersect(vec3 O, vec3 invD, HitRecord& record)
	{

		vec3 t0 = (this->bounds.min - O) * invD;
		vec3 t1 = (this->bounds.max - O) * invD;

		float tmin = Min(t0.x, t1.x);
		float tmax = Max(t0.x, t1.x);

		tmin = Max(tmin, Min(t0.y, t1.y));
		tmax = Min(tmax, Max(t0.y, t1.y));

		tmin = Max(tmin, Min(t0.z, t1.z));
		tmax = Min(tmax, Max(t0.z, t1.z));

		// early out
		if (tmin > record.t)
			return false;

		return tmax >= tmin && tmax >= 0;
	}

	void BVHNode::destroy()
	{
		if (this->isLeaf)
		{
			delete this;

			return;
		}

		this->left->destroy();
		this->right->destroy();

		delete this;
	}

	///////////////////
	///BVH
	/////////////////////

	BVH::BVH() 
	{
	}

	void BVH::ConstructBVH(const std::vector<std::unique_ptr<Object>> &objects, BVHNode* &node, uint*& objectIndices)
	{
		uint* indices = new uint[objects.size()];

		for (uint i = 0; i < objects.size(); i++)
			indices[i] = i;

		BVHNode* root = new BVHNode();

		root->first = 0;
		root->count = objects.size();
		CalcBounds(objects, root, indices);
		root->isLeaf = true;
		Subdivide(objects, root, indices);
		node = root;
		objectIndices = indices;
	}

	void BVH::CalcBounds(const std::vector<std::unique_ptr<Object>>& objects, BVHNode* &root, uint* &indices)
	{
		float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;
		float minX = INFINITY, minY = INFINITY, minZ = INFINITY;

		for (uint i = root->first; i < root->first + root->count; i++)
		{
			uint index = indices[i];
			minX = Min(objects[index]->aabb_ptr->min.x, minX);
			minY = Min(objects[index]->aabb_ptr->min.y, minY);
			minZ = Min(objects[index]->aabb_ptr->min.z, minZ);

			maxX = Max(objects[index]->aabb_ptr->max.x, maxX);
			maxY = Max(objects[index]->aabb_ptr->max.y, maxY);
			maxZ = Max(objects[index]->aabb_ptr->max.z, maxZ);
		}

		root->bounds.min = vec3(minX, minY, minZ);
		root->bounds.max = vec3(maxX, maxY, maxZ);
		root->bounds.CalcCenter();
	}

	void BVH::Subdivide(const std::vector<std::unique_ptr<Object>>& objects, BVHNode* &root, uint* &indices)
	{
		if (root->count < MAX_PRIMITIVES)
		{
			root->isLeaf = true;
			return;
		}

		root->left = new BVHNode();
		root->right = new BVHNode();

		Partition(objects, root, BINS_COUNT, indices);

		Subdivide(objects, root->left, indices);
		Subdivide(objects, root->right, indices);
		root->isLeaf = false;
	}

	void BVH::Partition(const std::vector<std::unique_ptr<Object>>& objects, BVHNode* &root, uint binCount, uint* &indices)
	{
		std::vector<float> centers;
		std::vector<float> tempCenters;
		float cMin, cMax, tempCMin, tempCMax;
		uint axis0 = 0;

		for (uint axis = 0; axis < 3; axis++)
		{
			centers.clear();

			for (uint i = root->first; i < root->first + root->count; i++)
			{
				uint index = indices[i];

				if (axis == 0)
					centers.push_back(objects[index]->aabb_ptr->center.x);
				if (axis == 1)
					centers.push_back(objects[index]->aabb_ptr->center.y);
				if (axis == 2)
					centers.push_back(objects[index]->aabb_ptr->center.z);
			}

			for (uint i = 0; i < centers.size(); i++)
			{
				if (i == 0)
				{
					cMin = centers[i];
					cMax = centers[i];
				}
				else
				{
					if (centers[i] < cMin)
						cMin = centers[i];

					if (centers[i] > cMax)
						cMax = centers[i];
				}
			}

			if (axis == 0)
			{
				tempCenters = centers;
				tempCMin = cMin;
				tempCMax = cMax;
			}
			else
			{
				if (cMax - cMin > tempCMax - tempCMin)
				{
					tempCenters = centers;
					axis0 = axis;
					tempCMin = cMin;
					tempCMax = cMax;
				}
			}
		}

		const auto midPoint = (tempCMin + tempCMax) * 0.5f;

		uint countl = 0, countr = 0;
		for (uint i = root->first; i < root->first + root->count; i++)
		{
			if (axis0 == 0)
			{
				if (objects[i]->aabb_ptr->center.x < midPoint)
				{
					countl++;
				}
			}
			else if (axis0 == 1)
			{
				if (objects[i]->aabb_ptr->center.y < midPoint)
				{
					countl++;
				}
			}
			else
			{
				if (objects[i]->aabb_ptr->center.z < midPoint)
				{
					countl++;
				}
			}
		}

		countr = root->count - countl;

		root->left->first = root->first;
		root->left->count = countl;
		CalcBounds(objects, root->left, indices);

		root->right->first = root->first + countl;
		root->right->count = countr;
		CalcBounds(objects, root->right, indices);
	}

	bool BVH::Traverse(const Ray& r, HitRecord& record, BVHNode* node, const std::vector<std::unique_ptr<Object>>& objects, float t_min, float t_max, bool &hitIs, uint* objectIndices)
	{
		if (!node->intersect(r.origin(), r.direction().invert(), record))
			return false;

		if (node->isLeaf)
		{
			HitRecord temp_record;
			auto closest_so_far = t_max;

			for (uint i = node->first; i < node->first + node->count; i++)
			{
				uint index = objectIndices[i];
				if(objects[index]->Hit(r, t_min, closest_so_far, temp_record))
				{
					closest_so_far = temp_record.t;
					record = temp_record;
					hitIs = true;
				}
			}
		}
		else
		{
			Traverse(r, record, node->left, objects, t_min, t_max, hitIs, objectIndices);
			Traverse(r, record, node->right, objects, t_min, t_max, hitIs, objectIndices);
		}

		return hitIs;
	}
}
#ifndef KD_TREE
#define KD_TREE

#include <memory>

#include "AABB.hpp"
#include "../common.hpp"
#include "../models/Primitive.hpp"

static const size_t MAX_DEPTH = 20;
static const size_t SIZE = 5;
static const float COST_INTERSECT = 1.0f;
static const float COST_TRAVEL = 0.3f;

class Node {
private:
	Node *__left_ptr, *__right_ptr;

	vector<const Primitive *> __primitives;
	AABB __box;
	float __split_point;
	float __axis;

public:
	Node(const AABB &box, const vector<const Primitive *> &primitives, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, size_t depth);

	~Node() {
		if (__left_ptr != 0) {
			delete __left_ptr;
			delete __right_ptr;
		}
	}

	bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color, Primitive const* &hit_ptr) const;

	bool intersect(const Ray &ray, float len, unsigned long hash_code) const;

private:
	inline void nearPtr(const Ray &ray, Node* &near_ptr, Node* &far_ptr, float t_near, float t_far) const;

	inline float splitCost(const vector<const Primitive *> &primitives, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int &left, int &right);

	inline float calculateCost(const vector<const Primitive *> &primitives, float split_point, int &left, int &right) const;

	inline void splitBox(AABB &left_box, AABB &right_box, float split_point) const;
};

class KD_Tree {
private:
	shared_ptr<Node> __root;

public:
	KD_Tree(): __root(0) {}

	KD_Tree(const vector<const Primitive *> &primitives) {
		size_t size = primitives.size();
		vector<const Primitive *> model_ptrs(size);
		vector<vec3> min_ps(size); vector<vec3> max_ps(size);
		vector<const vec3 *> min_ptrs(size); vector<const vec3 *> max_ptrs(size);

		for (size_t i = 0; i < size; i ++) {
			model_ptrs[i] = primitives[i];
			min_ps[i] = primitives[i]->getMinP();
			max_ps[i] = primitives[i]->getMaxP();

			min_ptrs[i] = &min_ps[i];
			max_ptrs[i] = &max_ps[i];
		}

		vec3 min_p(numeric_limits<float>::max()), max_p(numeric_limits<float>::min());
		for (size_t i = 0; i < size; i ++) {
			min_p = glm::min(min_p, min_ps[i]);
			max_p = glm::max(max_p, max_ps[i]);
		}
		AABB box = AABB((min_p + max_p) / 2.0f, max_p - min_p);
		__root = shared_ptr<Node>(new Node(box, model_ptrs, min_ptrs, max_ptrs, 0));
	}

	inline bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color, Primitive const* &hit_ptr) const {
		return __root->intersect(ray, t, hit_normal, hit_surface_color, hit_ptr);
	}

	inline bool intersect(const Ray &ray, float len, unsigned long hash_code) const {
		return __root->intersect(ray, len, hash_code);
	}
};

#endif
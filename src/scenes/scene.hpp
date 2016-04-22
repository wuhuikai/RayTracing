#ifndef SCENE
#define SCENE

#include "../common.hpp"
#include "../models/basic_model.hpp"

class Scene {
private:
	vector<BasicModel *> models;
	vector<BasicModel *> lights;
	vec3 backgroud_color;
	unsigned char max_recursive;

public:
	Scene(const vec3 &backgroud_color, unsigned char max_recursive): backgroud_color(backgroud_color), max_recursive(max_recursive) {}

	~Scene() {
		for (vector<BasicModel *>::iterator it = models.begin(); it != models.end(); ++it) {
			delete (*it);
		}
	}

	inline void addModel(BasicModel *model) {
		models.push_back(model);
	}

	inline void addLight(BasicModel *light) {
		models.push_back(light);
		lights.push_back(light);
	}

	inline const vector<BasicModel *>& getModels() const {
		return models;
	}

	inline const vector<BasicModel *>& getLights() const {
		return lights;
	}

	inline vec3 getBackgroundColor() const {
		return backgroud_color;
	}

	inline unsigned char getMaxRecursive() const {
		return max_recursive;
	}
};

#endif
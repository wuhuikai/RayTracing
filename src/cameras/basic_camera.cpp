#include "basic_camera.hpp"

const float BasicCamera::IM_DIST = 1.0f;

void thread_raytracing(const Ray &ray, vec3 *color, unsigned long *hash_code, Scene const* scene_ptr) {
	float min_t;
	*color = raytracing(ray, 0, min_t, *hash_code, scene_ptr);
}

vec3 raytracing(const Ray &ray, unsigned short recursive_count, float &min_t, unsigned long &hash_code, Scene const* scene_ptr) {
	min_t = numeric_limits<float>::max();

	hash_code += recursive_count;

	if (recursive_count > scene_ptr->getMaxRecursive())
		return vec3(0.0);

	const vector<const Light *> &lights = scene_ptr->getLights();
	const vec3 &backgroud_color = scene_ptr->getBackgroundColor();

	Primitive const* hit_ptr = 0;
	vec3 normal; vec3 surface_color;

	if (!scene_ptr->intersect(ray, min_t, normal, surface_color, hit_ptr))
		return backgroud_color;

	hash_code += hit_ptr->getHashCode();
	const Material &material = hit_ptr->getMaterial();

	if (material.isLight())
		return hit_ptr->getSurfaceColor();
	vec3 color = backgroud_color * surface_color;
	float diffuse = material.getDiffuse();
	float specular = material.getSpecular();

	const vec3 &ray_position = ray.getPosition();
	const vec3 &ray_direction = ray.getDirection();
	vec3 hit_position = ray_position + min_t * ray_direction;
	vec3 reflect_ray_dir = reflect(ray_direction, normal);

	for (auto it = lights.begin(); it != lights.end(); ++it) {
		vec3 hit2light_dir;
		float shade_idx = (*it)->calcShade(scene_ptr, hit_position, hit2light_dir);
		if (shade_idx <= EPSILON)
			continue;

		hash_code += (unsigned long)((*it)->getHashCode() * shade_idx);
		if (diffuse > 0) {
			float difuse_radio = dot(hit2light_dir, normal);
			if (difuse_radio > 0) {
				color += difuse_radio * surface_color * (*it)->getEmissionColor() * diffuse * shade_idx;
			}
		}

		if (specular > 0) {
			float specular_radio = dot(reflect_ray_dir, hit2light_dir);
			if (specular_radio > 0) {
				color += glm::pow(specular_radio, material.getSpecularPower()) * (*it)->getEmissionColor() * specular * shade_idx;
			}
		}
	}

	float reflection = material.getReflection();
	float dist;
	if (reflection > 0) {
		color += reflection * raytracing(Ray(hit_position + REFLACT_EPSILON * reflect_ray_dir, reflect_ray_dir), recursive_count + 1, dist, hash_code, scene_ptr) * surface_color;
	}

	bool outside = dot(normal, -ray_direction) > 0 ? true : false;
	if (!outside) {
		normal = -normal;
	}
	float transparency = material.getTransparency();
	if (transparency > 0) {
		float refraction_radio = material.getRefractionRadio();
		float r_index = outside ? 1.0f / refraction_radio : refraction_radio;
		float cosI = -dot( normal, ray_direction );
		float cosT2 = 1.0f - r_index * r_index * (1.0f - cosI * cosI);
		if (cosT2 <= 0)
			return color;

		vec3 refract_dir = (r_index * ray_direction) + (r_index * cosI - sqrt( cosT2 )) * normal;

		vec3 refract_color = raytracing(Ray(hit_position + EPSILON * refract_dir, refract_dir), recursive_count + 1, dist, hash_code, scene_ptr);
		vec3 absorbance_color = glm::exp( surface_color * material.getAbsorbance() * -dist);
		color += refract_color * absorbance_color;
	}

	return color;
}
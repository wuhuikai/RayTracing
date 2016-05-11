#include <opencv2/highgui/highgui.hpp>

#include "common.hpp"

#include "models/Material.hpp"
#include "models/PointLight.hpp"
#include "models/AreaLight.hpp"
#include "models/Texture.hpp"
#include "models/SphereModel.hpp"
#include "models/Mesh.hpp"
#include "models/model_util.hpp"

#include "cameras/PinHoleCamera.hpp"
#include "cameras/DepthCamera.hpp"
#include "scenes/scene.hpp"

Mat scene_a();

int main() {
	//                                                              color, diffuse, specular, specular_power, reflection, transparency, refraction_radio, absorbance
	// scene.addModel(Plane (vec3(13, 7.4, 5.5), vec3(-13, 7.4, 5.5), vec3(-13, 7.4, -29),
	//                       vec3(13, 7.4, -29), 						Material(vec3(0.4, 0.7, 0.7), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(Plane (vec3(-13, -4.4, -8), vec3(13, -4.4, -16), vec3(13, 7.4, -16),
	//                       vec3(-13, 7.4, -8), 			 			Material(vec3(0.5, 0.3, 0.5), 0.6, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(Plane (vec3(-13, -4.4, 5.5), vec3(13, -4.4, 5.5), vec3(13, -4.4, -29),
	//                       vec3( -13, -4.4, -29),					Material(vec3(0.4, 0.3, 0.3), 1.0, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(new SphereModel(vec3(2, 0.8, -3), 2.5, 			Material(vec3(0.7, 0.7, 1.0), 0.0, 0.0, 00, 0.2, 0.8, 1.3, 0.15)));
	// scene.addModel(new SphereModel(vec3(-5.5, -0.5, -7), 2, 			Material(vec3(0.7, 0.7, 1.0), 0.1, 0.0, 00, 0.5, 0.0, 0.0, 0.00)));
	// scene.addModel(new SphereModel(vec3(-1.5, -3.8, -1), 1.5, 		Material(vec3(1.0, 0.4, 0.4), 0.1, 1.0, 20, 0.0, 0.3, 1.1, 0.15)));
	// scene.addModel(Cube(vec3(-2, 0.8, -3), vec3(1.5), 				Material(vec3(0.0, 1.0, 0.0), 1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));
	// scene.addModel(new Mesh("objs/cube.obj", 						Material(vec3(1.0, 0.0, 0.0), 1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));

	// for (int x = 0; x < 8; x++) {
	// 	for (int y = 0; y < 7; y++) {
	// 		scene.addModel(new SphereModel(vec3(-4.5 + x * 1.5, -4.3 + y * 1.5, -10),
	// 		                               0.3,						Material(vec3(0.3, 1.0, 0.4), 0.6, 0.6, 20, 0.0, 0.0, 0.0, 0.0)));
	// 	}
	// }

	// scene.addLight(new PointLight(vec3( 0, 5, -5), 0.1, vec3(0.4, 0.4, 0.4)));
	// scene.addLight(new PointLight(vec3(-3, 5, -1), 0.1, vec3(0.6, 0.6, 0.8)));
	// scene.addLight(new PointLight(vec3(-2, 2,  5), 0.1, vec3(1.0, 1.0, 1.0)));



	// PinHoleCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	// DepthCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);


	// camera.render(result, 15, 16, 40, 16);
	// camera.render(result, 4, 16, 1, 1);

	Mat result = scene_a();
	imshow("Raytracing", result);

	vector<int> params;
	params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	params.push_back(0);
	imwrite("Raytracing.png", result, params);

	waitKey();
	return 0;
}

Mat scene_a() {
	/*************************Default params**********************/
	float radio = 4.0 / 3.0;
	size_t width = 640;

	/*************************Init Scene**********************/
	Scene scene(vec3(0), 2);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);

	/*************************Add models**********************/

	//background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	//ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 2.5, 2.5, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	// Spheres
	scene.addModel(new SphereModel(vec3(0.0, 0.0, -4), 0.5, 			Material(vec3(0.0, 1.0, 0.0), 0.1, 0.0, 00, 0.7, 0.0, 0.0, 0.00)));
	scene.addModel(new SphereModel(vec3(1.3, 0.0, -3), 0.5, 			Material(vec3(1.0, 1.0, 1.0), 0.0, 0.0, 00, 0.0, 1.0, 1.3, 0.00)));
	scene.addModel(new SphereModel(vec3(-2.0, 0.0, -5), 1.0, 			Material(planet, 1.0, 1.0, vec3(1.0, 1.0, 1.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00)));

	/*************************Add Light**********************/
	// Point Light
	scene.addLight(new PointLight(vec3(-2, 3.5, 0), 0.1, vec3(2.0)));

	// Area Light
	// scene.addLight(new AreaLight(vec3(-2, 3.5, 0), 1, 1, vec3(2.0)));

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);


	// Depth Camera
	// DepthCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	camera.setScene(&scene);
	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 4);

	//Depth Camera
	// camera.render(result, 5, 8, 4, 16);

	return result;
}
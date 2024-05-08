//#pragma once
//
//#include <vector>
//#include "framework/entities/entity.h"
//#include "camera.h"
//#include "graphics/shader.h"
//#include "utils.h"
//#include <fstream>
//#include "input.h"
//#include "game/game.h"
//
//
////Texture* CubemapFromHDRE(const char* filename);
//
//struct sRenderData {
//	Texture* texture = nullptr;
//	Shader* shader = nullptr;
//	std::vector<Matrix44> models;
//};
//
//class Scene
//{
//public:
//	Camera* camera;
//	Entity* track;
//	Texture* skybox;
//	int window_width;
//	int window_height;
//	std::map<std::string, sRenderData> meshes_to_load;
//	Scene(Camera* camera);
//	virtual void renderScene() {}
//	virtual void update(float dt) {}
//	bool parseScene(const char* filename, Shader* shader);
//	bool parseB2(const char* filename, B2Entity* b2, Shader* shader);
//};
//
//class PlayScene : public Scene
//{
//public:
//	B2Entity* b2;
//	Shader* shader;
//	float angle;
//	float mouse_speed;
//	bool mouse_locked;
//	long long best_time;
//	std::vector<std::string> tiempos;
//
//	//HSAMPLE hSample;	// Handler to store one sample	
//	//HCHANNEL hSampleChannel; // Handler to store one channel
//
//	PlayScene(Camera* camera);
//
//	void setupScene(int window_width, int window_height);
//	void renderScene();
//	void update(float dt);
//
//};
//
//class B2SetupScene : public Scene {
//public:
//	Shader* shader;
//	//Button* toggle_camera;
//	bool oneClick = true;
//	bool first_person_camera = false;
//	float downforce = 1;
//
//	B2SetupScene(Camera* camera);
//	void setupScene(int window_width, int window_height);
//	void renderScene();
//	void update(float dt);
//};
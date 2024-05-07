#include "scene.h"

Scene::Scene(Camera* camera) {
	this->camera = camera;
	track = new Entity();

};



bool Scene::parseScene(const char* filename, Shader* shader)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors
	//meshes_to_load["meshes/example.obj"] = { Texture::Get("texture.tga"), Shader::Get("shader.vs", "shader.fs") };
	meshes_to_load.clear();
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	int progress = 0;
	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {
		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			//Esto deberia ser un instancedPrefabEntity o algo asi
			PrefabEntity* new_entity = new PrefabEntity(mesh_name.c_str(), Mesh::Get(mesh_name.c_str()), shader, render_data.texture);
			// Add all instances
			//new_entity->models = render_data.models;
			// Add entity to scene root
			track->addChild(new_entity);
		}
		// Create normal entity
		else {
			PrefabEntity* new_entity = new PrefabEntity(mesh_name.c_str(), Mesh::Get(mesh_name.c_str()), shader, render_data.texture);
			new_entity->model = render_data.models[0];

			if (mesh_name.find("_0groove_01") != std::string::npos) new_entity->minimap_render = true;
			else new_entity->minimap_render = false;
			// Add entity to scene root
			track->addChild(new_entity);
		}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}

bool Scene::parseB2(const char* filename, B2Entity* car, Shader* shader)
{
	// You could fill the map manually to add shader and texture for each mesh
	// If the mesh is not in the map, you can use the MTL file to render its colors
	//meshes_to_load["meshes/example.obj"] = { Texture::Get("texture.tga"), Shader::Get("shader.vs", "shader.fs") };
	meshes_to_load.clear();
	std::cout << " + Scene loading: " << filename << "..." << std::endl;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}


	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/" + data.first;
		sRenderData& render_data = data.second;

		// No transforms, anything to do here
		if (render_data.models.empty())
			continue;

		// Create instanced entity
		if (render_data.models.size() > 1) {
			//Esto deberia ser un instancedPrefabEntity o algo asi
			PrefabEntity* new_entity = new PrefabEntity(mesh_name.c_str(), Mesh::Get(mesh_name.c_str()), shader, render_data.texture);
			// Add all instances
			//new_entity->models = render_data.models;
			// Add entity to scene root
			car->addChild(new_entity);
		}
		// Create normal entity
		else {
			PrefabEntity* new_entity = new PrefabEntity(mesh_name.c_str(), Mesh::Get(mesh_name.c_str()), shader, render_data.texture);
			new_entity->model = render_data.models[0];
			// Add entity to scene root
			car->addChild(new_entity);
		}
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}
PlayScene::PlayScene(Camera* camera) : Scene(camera) {
	b2 = nullptr;
	angle = 0;
	mouse_speed = 10.0f;
	mouse_locked = false;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phongobj.fs");
	//skybox = CubemapFromHDRE("data/panorama.hdre");
}

//Texture* CubemapFromHDRE(const char* filename)
//{
//	HDRE* hdre = HDRE::Get(filename);
//	if (!hdre)
//		return NULL;
//
//	Texture* texture = new Texture();
//	if (hdre->getFacesf(0))
//	{
//		texture->createCubemap(hdre->width, hdre->height, (Uint8**)hdre->getFacesf(0),
//			hdre->header.numChannels == 3 ? GL_RGB : GL_RGBA, GL_FLOAT);
//		for (int i = 1; i < hdre->levels; ++i)
//			texture->uploadCubemap(texture->format, texture->type, false,
//			(Uint8**)hdre->getFacesf(i), GL_RGBA32F, i);
//	}
//	else
//		if (hdre->getFacesh(0))
//		{
//			texture->createCubemap(hdre->width, hdre->height, (Uint8**)hdre->getFacesh(0),
//				hdre->header.numChannels == 3 ? GL_RGB : GL_RGBA, GL_HALF_FLOAT);
//			for (int i = 1; i < hdre->levels; ++i)
//				texture->uploadCubemap(texture->format, texture->type, false,
//				(Uint8**)hdre->getFacesh(i), GL_RGBA16F, i);
//		}
//	return texture;
//}


void PlayScene::setupScene(int window_width, int window_height) {
	this->window_height = window_height;

	this->window_width = window_width;

	parseScene("data/track.scene", shader);
	//parseScene("data/aaa/paul/paul.scene", shader);

	//speed values
	//los parametros se pasan dentro de dos structs para tener constructores mas sencillos
	//sSpeedParameters sp = sSpeedParameters();
	//sp.max_speed = 65;
	//sp.max_acceleration = 6.5f;
	//sp.max_bracking = 15.5;
	//sp.downforce = 2.2;

	//sTurningParameters tp = sTurningParameters();
	//tp.max_angle = 30;
	//tp.turning_acceleration = 4;
	//tp.max_turn_speed = .6f;
	//tp.centering_acceleration = 0.1;
	//tp.turning_speed_mult = 2.0f;

	b2 = new B2Entity(
		"b2",
		Vector3(200, 3, 500),
		shader,
		0.4
	);

	parseB2("data/car2.scene", b2, shader);

	Vector3 b2_pos = b2->model.getTranslation();

	//b2->sectors[0] = false;
	//b2->sectors[1] = false;
	//b2->track_limits = true;

	////Mejor Time
	//std::ifstream archivo("data/tiempo.txt");
	//std::string tiempo;

	//if (!archivo) {
	//	std::cerr << "Error al abrir el archivo" << std::endl;
	//	std::cout << best_time << std::endl;
	//}
	//else {
	//	while (std::getline(archivo, tiempo)) {
	//		tiempos.push_back(tiempo);
	//	}

	//	archivo.close();
	//}

	//create our first person camera
	//camera->lookAt(Vector3(car_pos.x, car_pos.y + 3.6, car_pos.z - 3), Vector3(car_pos.x, car_pos.y + 3.39, car_pos.z + 1), Vector3(0.f, 1.f, 0.f));
	//camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	//create our third person camera	
	camera->lookAt(Vector3(b2_pos.x, b2_pos.y + 3, b2_pos.z - 4), Vector3(b2_pos.x, b2_pos.y + 3, b2_pos.z + 1), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	// Play channel
	//Audio::Play("data/sounds/DejaVu.wav");
	//Audio::Play3D("data/sounds/F1_sonido.wav", Vector3(200, 0, 500));
}

void PlayScene::renderScene() {
	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!shader)
		return;

	//generateSkybox();

	//root renderiza la pista
	track->render(camera);
	//renderiza el coche
	b2->render(camera);

	//Draw the floor grid
	//drawGrid();
	//Draw GUI
	//ui->drawMinimap(car, track);
	//ui->drawGUI();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//drawText(370, 550, car->getGear(), Vector3(1, 1, 1), 2);
	//drawText(370, 570, std::to_string(car->getRotationSpeed()), Vector3(1, 1, 1), 2);

}

void PlayScene::update(float dt) {

	float speed = dt * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}


	Vector3 b2_pos = b2->model.getTranslation();
	int dir = 0;
	int turn = 0;
	Vector3 prevPos = b2->model.getTranslation();
	//async input to move the camera around
	//if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		dir++;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		dir--;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::gamepads[0].direction & PAD_LEFT) {
		turn++;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::gamepads[0].direction & PAD_RIGHT) {
		turn--;
	}
	//if (Input::isKeyPressed(SDL_SCANCODE_R) || Input::gamepads[0].isButtonPressed(X_BUTTON)) {
	//	b2->goBackwards();
	//}
	//if (Input::isKeyPressed(SDL_SCANCODE_F) || Input::gamepads[0].isButtonPressed(Y_BUTTON)) {
	//	b2->goForwards();
	//}
	//b2->move(dir, turn, dt, camera, Game::instance->car_setup_scene->first_person_camera);
	//std::vector<sCollisionData> collisions = std::vector<sCollisionData>();
	//checkCarCollisions(collisions);
	//Vector3 current_car_pos = b2->getPosition();
	//if (!collisions.empty()) {
	//	for each (sCollisionData collision in collisions)
	//	{
	//		if (collision.type == FLOOR)
	//			car->model.setPosition(current_car_pos.x, collisions[0].colPoint.y + 1, current_car_pos.z);
	//		if (collision.type == WALL) {
	//			car->model.setPosition(prevPos.x, prevPos.y, prevPos.z);
	//			car->speed = 0;
	//			Audio::Stop(Game::instance->channel);

	//			channel = Audio::Play("data/sounds/crash.wav");
	//			//TODO trigger game over
	//			Game::instance->current_scene = Game::instance->game_over;
	//		}
	//	}
	//}
	//std::cout << current_car_pos.x << " " << current_car_pos.y << " " << current_car_pos.z << std::endl;
	//to navigate with the mouse fixed in the middle
	//if (mouse_locked)
	//	Input::centerMouse();
}

B2SetupScene::B2SetupScene(Camera* camera) : Scene(camera) {
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phongobj.fs");
	//skybox = CubemapFromHDRE("data/panorama.hdre");
}

void B2SetupScene::setupScene(int window_width, int window_height) {
	this->window_height = window_height;
	this->window_width = window_width;

	parseScene("data/track.scene", shader);

	Vector3 track_pos = track->model.getTranslation();

	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void B2SetupScene::renderScene() {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (!shader)
		return;

	//generateSkybox();

	//root renderiza la pista
	track->render(camera);

	drawText(window_width / 2, (window_height / 2) - 50, std::to_string(downforce), Vector3(0), 2);
	drawText(window_width / 2, (window_height / 2) + 90, first_person_camera ? "first Person" : "third Person", Vector3(0), 2);
	//if (SDL_NumJoysticks() > 0) drawText(240, 500, "Pulsa A para empezar", Vector3(1, 1, 1), 3);
	//else drawText(250, 500, "Clica para empezar", Vector3(1, 1, 1), 3);
}

void B2SetupScene::update(float dt) {
	camera->lookAt(Vector3(200, 15, 498), Vector3(220, 3, 500), Vector3(0.f, 1.f, 0.f));

	if (Input::isMousePressed(SDL_BUTTON(1)) && oneClick) {
		oneClick = false;
		Vector2 mousePos = Input::mouse_position;
		std::cout << mousePos.x << " " << mousePos.y << std::endl;
		
	}
	if (!Input::isMousePressed(SDL_BUTTON(1)))
		oneClick = true;
}
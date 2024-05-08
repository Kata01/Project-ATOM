#include "game.h"
#include "framework/utils.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/fbo.h"
#include "graphics/shader.h"
#include "framework/input.h"

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
float mouse_speed = 100.0f;

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
	: plane_mesh(nullptr), plane_shader(nullptr), plane_texture(nullptr) // Initialize pointers to nullptr
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	// OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	plane_mesh = new Mesh();  // Create a new mesh
	plane_mesh->createPlane(10.0f);  // Example: create a plane of size 10x10 units

	// Load the shader
	plane_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");  // Load basic shader

	// Load the texture (optional)
	plane_texture = Texture::Get("data/textures/plane_texture.tga");  // Example texture

	// Create our camera
	camera = new Camera();
	/*play_scene = new PlayScene(camera);
	play_scene->setupScene(window_width, window_height);

	b2_setup_scene = new B2SetupScene(camera);
	b2_setup_scene->setupScene(window_width, window_height);

	current_scene = play_scene;*/
	// Load one texture using the Texture Manager
	//texture = Texture::Get("data/textures/texture.tga");

	// Example of loading Mesh from Mesh Manager
	mesh = Mesh::Get("data/meshes/box.ASE");

	// Example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	// Hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera as default
	camera->enable();

	// Set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	// Create model matrix for cube
	Matrix44 m;
	m.rotate(angle*DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));

	if (plane_shader) {
		// Enable shader
		plane_shader->enable();

		// Set uniforms
		plane_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		plane_shader->setUniform("u_color", Vector4(1, 1, 1, 1)); // White color

		if (plane_texture) {
			plane_shader->setUniform("u_texture", plane_texture, 0); // Bind texture
		}

		// Define a transformation matrix for the plane
		Matrix44 plane_model;
		plane_model.setTranslation(0.0f, -5.0f, 0.0f); // Position it at some point in the scene
		plane_shader->setUniform("u_model", plane_model); // Apply the transformation

		// Render the plane mesh
		if (plane_mesh) {
			plane_mesh->render(GL_TRIANGLES); // Draw the plane
		}

		plane_shader->disable(); // Disable shader
	}

	// Draw the floor grid
	drawGrid();

	// Render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	// Swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	// Example
	angle += (float)seconds_elapsed * 10.0f;

	// Mouse input to rotate the cam
	if (Input::isMousePressed(SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	// Async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
		SDL_SetRelativeMouseMode((SDL_bool)(mouse_locked));
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{

}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1f : 0.9f;
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}


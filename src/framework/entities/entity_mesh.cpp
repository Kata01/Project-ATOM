#include "entity_mesh.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "framework/camera.h"
#include "graphics/material.h"

EntityMesh::EntityMesh(Mesh* mesh, const Material& material, const std::string& name) {
	this->mesh = mesh;
	this->material = material;
	this->name = name;
};
void EntityMesh::render(Camera* camera) {

	// Get the last camera that was activated 
	camera = Camera::current;

	// Enable shader and pass uniforms 
	material.shader->enable();
	material.shader->setUniform("u_model", model);
	material.shader->setUniform("u_viewproj", camera->viewprojection_matrix);
	material.shader->setTexture("u_texture", texture, 1);


	material.shader->setUniform("u_model", getGlobalMatrix());

	// Render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	material.shader->disable();

	Entity::render(camera);
};

void EntityMesh::update(float deltatime) {
	Entity::update(deltatime);
}

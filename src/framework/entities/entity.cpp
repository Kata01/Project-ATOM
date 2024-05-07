#include "entity.h"
#include "framework/camera.h"

#include <algorithm>

void Entity::render(Camera* camera)
{
	for (int i = 0; i < children.size(); ++i) {
		children[i]->render(camera);
	}
}

Entity::Entity(std::string name, float scale) {
	this->scale = scale;
	this->name = name;
	parent = nullptr;
}

Entity::Entity() {
	parent = nullptr;
	scale = 1;
}
void Entity::update(float delta_time)
{
	for (int i = 0; i < children.size(); ++i) {
		children[i]->update(delta_time);
	}
}

void Entity::addChild(Entity* child)
{
	if (child->parent) {
		std::cerr << "Child has already a parent, remove it first!" << std::endl;
		return;
	}

	// Checks if it's already a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) {
		std::cerr << "Entity is already one of the children!" << std::endl;
		return;
	}

	child->parent = this;
	children.push_back(child);
}

void Entity::removeChild(Entity* child)
{
	// Checks if it's a child
	auto it = std::find(children.begin(), children.end(), child);
	if (it == children.end()) {
		std::cerr << "Entity is not a child!!" << std::endl;
		return;
	}

	children.erase(it);
	child->parent = nullptr;
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent)
		return model * parent->getGlobalMatrix();
	return model;
}

float Entity::distance(Entity* e)
{
	return model.getTranslation().distance(e->model.getTranslation());
}

//PrefabEntity::PrefabEntity(std::string name, Vector3 position, const char* meshf, const char* texturef, Shader* shader, float scale) : Entity(name, scale) {
//	mesh = Mesh::Get(meshf);
//	if (texturef) texture = Texture::Get(texturef); else texture = new Texture();
//	model.setTranslation(position.x, position.y, position.z);
//	this->shader = shader;
//	isDynamic = false;
//}
//
PrefabEntity::PrefabEntity(std::string name, Mesh* mesh, Shader* shader, Texture* texture, float scale) : Entity(name, scale) {
	this->mesh = mesh;
	this->shader = shader;
	if (texture) this->texture = texture;
	else this->texture = new Texture();
	model.setIdentity();
	isDynamic = false;
}

PrefabEntity::PrefabEntity(std::string name, Vector3 position, Shader* shader, float scale) : Entity(name, scale) {
	mesh = nullptr;
	texture = nullptr;
	model.setTranslation(position.x, position.y, position.z);
	this->shader = shader;
	isDynamic = false;
}

void PrefabEntity::render(Camera* camera) {

	const Matrix44& globalMatrix = getGlobalMatrix();
	Vector3 box_center = globalMatrix * mesh->box.center;
	Vector3 half_size = mesh->box.halfsize;

	//mesh->renderBounding(globalMatrix);

	// Discard objects whose bounding sphere 
	// is not inside the camera frustum
	if (camera->testBoxInFrustum(box_center, half_size) == false) {
		//std::cout << "a" << std::endl;
		return;
	}


	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
	shader->enable();


	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_model", globalMatrix);
	shader->setUniform("lightPos", Vector3(10, 10, 10));
	shader->setUniform("Ia", 0.7f);
	shader->setUniform("Kd", 0.9f);
	shader->setUniform("Ks", 0.4f);

	//do the draw call
	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();

	for (int i = 0; i < children.size(); i++)
		children[i]->render(camera);
}

B2Entity::B2Entity(std::string name, Vector3 position, Shader* shader, float scale) : PrefabEntity(name, position, shader, scale) {
	this->model.scale(0.5, 0.5, 0.5);
	speed = 0;
	angle = 0;
	rotation_speed = 0;
}


void B2Entity::render(Camera* camera) {
	for (int i = 0; i < children.size(); i++) {
		PrefabEntity* ent = (PrefabEntity*)children[i];
		//Matrix44 m = ent->model * model;
		//ent->model = ent->getGlobalMatrix();
		ent->render(camera);
	}
}
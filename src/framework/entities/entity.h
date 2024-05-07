#pragma once

#include "framework/includes.h"
#include "framework/framework.h" 
#include "graphics/mesh.h"
#include "framework/camera.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

class Camera;

class Entity {

public:

	Entity();
	Entity(std::string name, float scale = 1);

	std::string name;

	Matrix44 model;

	float scale;

	Entity* parent = nullptr;
	std::vector<Entity*> children;

	void addChild(Entity* child);
	void removeChild(Entity* child);

	// Methods that should be overwritten
	// by derived classes 
	virtual void render(Camera* camera);
	virtual void update(float delta_time);

	// Some useful methods
	Matrix44 getGlobalMatrix();
	float distance(Entity* e);
};

class EntityCollider {
public:
	bool isDynamic = false;
	int layer = 0;
	virtual bool  testCollision(Entity* entity) { return false; }
};

class PrefabEntity : public Entity, public EntityCollider {
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;
	bool minimap_render;

	//PrefabEntity(std::string name, Vector3 position, const char* meshf, const char* texturef, Shader* shader, float scale = 1);
	PrefabEntity(std::string name, Mesh* mesh, Shader* shader, Texture* texture, float scale = 1);
	PrefabEntity(std::string name, Vector3 position, Shader* shader, float scale = 1);

	virtual void render(Camera* camera);
};

class B2Entity : public PrefabEntity {
public:

	float speed;
	float angle;
	float rotation_speed;

	B2Entity(std::string name, Vector3 position, Shader* shader, float scale = 1);
	void render(Camera* camera);


private:

};
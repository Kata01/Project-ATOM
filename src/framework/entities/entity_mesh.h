#include "graphics/material.h"
#include "entity.h"
#include "framework/animation.h"


class EntityMesh : public Entity {

public:

	EntityMesh() {};
	EntityMesh(Mesh* mesh, const Material& material, const std::string& name = "");

	// Attributes of the derived class  
	Mesh* mesh = nullptr;

	Material material;

	Entity entity;

	Texture* texture = nullptr;
	Shader* shader = nullptr;
	Vector4 color;

	// Methods overwritten from base class
	void render(Camera* camera) override;
	void update(float elapsed_time) override;
};

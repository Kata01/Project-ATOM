#pragma once

#ifndef AIRPLANE_H
#define AIRPLANE_H


#include "framework/includes.h"
#include "framework/framework.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/camera.h"
#include "framework/entities/entity.h"


class B2 : public Entity {

public:

	static std::vector<B2*> b2;

	char type;
	Matrix44 model;
	float speed;

	B2();

	~B2();

	static void renderAll(Camera* camera);
	static void updateAll(float seconds_elapsed);
};

enum PlaneType{ B2plane };


// Active planes count
extern int active_b2;

// Function prototypes for rendering airplanes
void renderB2();

#endif // AIRPLANE_H

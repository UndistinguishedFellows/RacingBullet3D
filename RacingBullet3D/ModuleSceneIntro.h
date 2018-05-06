#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2
#define DIAMETER_WORLD 2500

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	p2List<Cube*> s;
	p2List<PhysBody3D*> sensor;

private:
	void CreateSphere(const vec3& position, float radius);
	void CreateCube(const vec3& position, const vec3& size, float angle = 0.0f, const vec3& rotAxis = vec3(1, 1, 1), bool sensor = false);

	Cube floor_cube;
	PhysBody3D* floor_sensor;

	p2List<Primitive*> primitives;
};
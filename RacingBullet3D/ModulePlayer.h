#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 10.0f * DEGTORAD
#define BRAKE_POWER 250.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	bool debug;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float nitro;
	int deaths;
	int last_deaths;
	float time;
	float last_time;
	float time_to_start;
	bool reset;
	unsigned int reset_timer;
	mat4x4 last_checkpoint_matrix;
	int checkpoint;

private:
	mat4x4 initial_matrix;
};
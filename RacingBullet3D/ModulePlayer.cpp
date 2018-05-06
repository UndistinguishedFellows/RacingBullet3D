#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	nitro = 50.0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	debug = false;
	reset = false;
	checkpoint = 0;

	deaths = 0;
	last_deaths = 0;
	time = 0.0f;
	last_time = 0.0f;
	time_to_start = 0.0f;

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width, connection_height + 0.5f, half_length);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width, connection_height + 0.5f, half_length);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-RIGHT ------------------------
	car.wheels[2].connection.Set(-half_width - 0.4f * wheel_width, connection_height + 0.2f, -half_length - 0.2f);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width + 0.5f;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-LEFT ------------------------
	car.wheels[3].connection.Set(half_width + 0.4f * wheel_width, connection_height + 0.2f, -half_length - 0.2f);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width + 0.5f;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 52, 10);

	vehicle->GetTransform(&initial_matrix);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (last_time == 0.0f)
	{
		time_to_start = SDL_GetTicks();

		last_time = SDL_GetTicks() - time_to_start;
	}//for better time accuracy

	time = SDL_GetTicks() - last_time;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT|| reset ||
		App->input->GetJButton(8) == KEY_DOWN)
	{
		brake = BRAKE_POWER;
	}

	else
	{
		if (App->input->GetJAxisValue() != 0)
			turn = TURN_DEGREES * -App->input->GetJAxisValue();

		if (App->input->GetJAccelValue() != 0 || App->input->GetJDeaccelValue() != 0)
		{
			if (App->input->GetJAccelValue() - App->input->GetJDeaccelValue() > 0)
			{
				if (vehicle->GetKmh() < 0.0f)
					acceleration = MAX_ACCELERATION * 2 * (App->input->GetJAccelValue() - App->input->GetJDeaccelValue());
				else
					acceleration = MAX_ACCELERATION * (App->input->GetJAccelValue() - App->input->GetJDeaccelValue());
			}
			else
			{
				if (vehicle->GetKmh() > 0.0f)
					acceleration = MAX_ACCELERATION * 2 * (App->input->GetJAccelValue() - App->input->GetJDeaccelValue());
				else
					acceleration = (MAX_ACCELERATION / 2) * (App->input->GetJAccelValue() - App->input->GetJDeaccelValue());
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 0.0f)
					acceleration = -MAX_ACCELERATION * 2;
				else
					acceleration = -MAX_ACCELERATION / 2;
			}

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() < 0.0f)
					acceleration = MAX_ACCELERATION * 2;
				else
					acceleration = MAX_ACCELERATION;
			}
		}
	}

	if (acceleration != 0.0f && nitro > 1.0f && (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetJButton(9) == KEY_DOWN))
	{
		acceleration = acceleration * 4;
		nitro--;
	}

	else if (nitro < 99.9)
		nitro += 0.1f;

	if ((App->input->GetKey(SDL_SCANCODE_F9) == KEY_REPEAT || App->input->GetJButton(5) == KEY_DOWN))
	{
		if (reset_timer == 0)
			reset_timer = SDL_GetTicks();
	}
	else
		reset_timer = 0;

	if (reset_timer != 0)
	{
		if (SDL_GetTicks() - reset_timer > 2500)
		{
			reset = true;
			reset_timer = 0;
		}
		else if (SDL_GetTicks() - reset_timer > 500)
			brake = BRAKE_POWER;
	}

	if (reset)
	{
		vehicle->vehicle->getRigidBody()->clearForces();
		vehicle->vehicle->getRigidBody()->clearVelocities();

		if (checkpoint == -1)
		{
			last_time = time;
			last_deaths = deaths;
			deaths = 0;
			checkpoint = 0;

			vehicle->SetTransform(&initial_matrix);
		}
		else
		{
			deaths++;

			if (checkpoint == 0)
				vehicle->SetTransform(&initial_matrix);
			else
				vehicle->SetTransform(&last_checkpoint_matrix);
		}

		nitro = 50.0;
		reset = false;
	}

	/*if (acceleration != 0)
	{
		if (vehicle->GetKmh() > 20)
			acceleration = acceleration / (vehicle->GetKmh() / 20);

		else if (vehicle->GetKmh() < -20)
			acceleration = acceleration / (-vehicle->GetKmh() / 20);
	}*/

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug == false)
	{
		App->camera->bodyToFollow = vehicle;
	}

	char title[150];
	sprintf_s(title, "%.1f Km/h   %.1f nitro   timer %.3f s    %d deaths   |   last time %.3f s   %d last deaths", vehicle->GetKmh(), nitro, time / 1000, deaths, last_time / 1000, last_deaths);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}




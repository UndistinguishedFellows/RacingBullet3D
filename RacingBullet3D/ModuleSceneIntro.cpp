#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	floor_cube.size = vec3(DIAMETER_WORLD, 0, DIAMETER_WORLD);
	floor_cube.SetPos(0, 2.5f, 0);

	floor_sensor = App->physics->AddBody(floor_cube, 0.0f);
	floor_sensor->SetAsSensor(true);
	floor_sensor->collision_listeners.add(this);

	CreateCube(vec3(0, 37, 100), vec3(6, 10, 1), 0.0f, vec3(0, 0, 0), true); //1r checkpoint


	CreateCube(vec3(0, 50, 0), vec3(10.0f, 1.0f, 30.0f));
	//0
	CreateCube(vec3(0, 49.16, 19.7f), vec3(10.0f, 1.0f, 10.0f), 10.0f, vec3(1, 0, 0));
	CreateCube(vec3(0, 47.04, 29.28f), vec3(10.0f, 1.0f, 10.0f), 15.0f, vec3(1, 0, 0));
	CreateCube(vec3(0, 44.39, 38.93), vec3(10.0f, 1.0f, 10.0f), 16.0f, vec3(1, 0, 0));
	CreateCube(vec3(0, 41.58, 48.4), vec3(10.0f, 1.0f, 10.0f), 17.0f, vec3(1, 0, 0));
	CreateCube(vec3(0, 38.5, 57.88), vec3(10.0f, 1.0f, 10.0f), 19.0f, vec3(1, 0, 0));
	//5
	CreateCube(vec3(0, 35.59, 67.45), vec3(10.0f, 1.0f, 10.0f), 14.5855f, vec3(1, 0, 0));
	CreateCube(vec3(0, 33.5, 77.13), vec3(10.0f, 1.0f, 10.0f), 9.4119f, vec3(1, 0, 0));
	CreateCube(vec3(0, 32.31, 86.76), vec3(10.0f, 1.0f, 10.0f), 4.9394f, vec3(1, 0, 0));
	CreateCube(vec3(0, 31.84, 101.7), vec3(10.0f, 1.0f, 20.0f));
	CreateCube(vec3(-0.7, 31.84, 115.75), vec3(10.0f, 1.0f, 10.0f), -8.98, vec3(0, 1, 0));
	//10
	CreateCube(vec3(-2.75, 31.84, 124.54), vec3(10.0f, 1.0f, 10.0f), -16.0787, vec3(0, 1, 0));
	CreateCube(vec3(-5.76, 31.84f, 133.51), vec3(10.0f, 1.0f, 10.0f), -110.9619, vec3(0, 1, 0));
	CreateCube(vec3(-9.6, 31.84f, 142.26), vec3(10.0f, 1.0f, 10.0f), -116.8159, vec3(0, 1, 0));
	CreateCube(vec3(-14.29, 31.84f, 150.39), vec3(10.0f, 1.0f, 10.0f), -123.727, vec3(0, 1, 0));
	CreateCube(vec3(-19.96, 31.84f, 158.02), vec3(10.0f, 1.0f, 10.0f), -129.9672, vec3(0, 1, 0));
	//15
	CreateCube(vec3(-26.36, 31.84f, 164.65), vec3(10.0f, 1.0f, 10.0f), -138.3579, vec3(0, 1, 0));
	CreateCube(vec3(-33.58, 31.84f, 170.26), vec3(10.0f, 1.0f, 10.0f), -147.1527, vec3(0, 1, 0));//
	CreateCube(vec3(-41.84, 31.84f, 175.07), vec3(10.0f, 1.0f, 10.0f), -153.0961, vec3(0, 1, 0));
	CreateCube(vec3(-50.16, 31.84f, 178.22), vec3(10.0f, 1.0f, 10.0f), -75.0569, vec3(0, 1, 0));
	CreateCube(vec3(-58.19, 31.75, 179.33), vec3(10.0f, 1.0f, 10.0f), 1.5, vec3(0, 0, 1));
	//20
	CreateCube(vec3(-68.01, 31.4, 179.33), vec3(10.0f, 1.0f, 10.0f), 2.5, vec3(0, 0, 1));
	CreateCube(vec3(-77.65, 30.77, 179.33), vec3(10.0f, 1.0f, 10.0f), 5, vec3(0, 0, 1));
	CreateCube(vec3(-86.6, 29.8, 179.33), vec3(10.0f, 1.0f, 10.0f), 7, vec3(0, 0, 1));
	//23
	CreateCube(vec3(-109.34, 28.75, 179.33), vec3(35.61924, 1.0f, 10.0f), 1.5, vec3(0, 0, 1));//27
	CreateCube(vec3(-132.04, 28.48, 179.33), vec3(10.0f, 1.0f, 10.0f), -2, vec3(0, 0, 1));//28
	CreateCube(vec3(-141.63, 30.09, 179.33), vec3(10.0f, 1.0f, 10.0f), -16.8073, vec3(0, 0, 1));
	CreateCube(vec3(-150.94, 32.92, 179.33), vec3(10.0f, 1.0f, 10.0f), -17, vec3(0, 0, 1));//
	//30
	CreateCube(vec3(-159.97, 35.52, 179.33), vec3(10, 1, 10), -15, vec3(0, 0, 1)); // End of first ramp


	CreateCube(vec3(-195, 42.6 + 8, 181.7), vec3(1, 16, 46), 0.0f, vec3(0, 0, 0), true);//2n checkpoint

	CreateCube(vec3(-349.5, 42.6, 181.7), vec3(320, 1, 50));
	CreateCube(vec3(-349.5, 54.6, 156.7), vec3(320, 25, 1));
	CreateCube(vec3(-349.5, 54.6, 206.7), vec3(320, 25, 1));
	CreateCube(vec3(-509.5, 54.6, 21.7), vec3(1, 25, 268.4883));
	//35
	CreateCube(vec3(-559.5, 54.6, 47.7), vec3(1, 25, 320));
	CreateCube(vec3(-534.5, 42.6, 47.7), vec3(50, 1, 320));
	CreateCube(vec3(-534.5, 54.6, 207.7), vec3(51.63527, 25, 1));//38r
	CreateCube(vec3(-534.5, 51.6, 32.7), vec3(10, 1, 100), 10.1549, vec3(1, 0, 0));//38
	CreateCube(vec3(-534.5, 57.6, -42.3), vec3(46, 30, 1), 0.0f, vec3(0, 0, 0), true);//3r checkpoint
	CreateCube(vec3(-534.5, 64.6, -42.3), vec3(10, 1, 55.37737), 9.596, vec3(1, 0, 0));
	CreateCube(vec3(-534.5, 69.6, -144.3), vec3(20, 1, 150));
	//40
	CreateCube(vec3(-534.5, 42.6, -149.3), vec3(40, 1, 75));
	CreateCube(vec3(-534.5, 42.6, -211.3), vec3(25, 1, 50));
	CreateCube(vec3(-534.5, 42.6, -294.3), vec3(15, 1, 117.1603));
	CreateCube(vec3(-534.5, 69.6, -242.3), vec3(60, 1, 49.29916));
	CreateCube(vec3(-556.5, 69.6, -294.3), vec3(15, 1, 117.1603));
	//45
	CreateCube(vec3(-512.5, 69.6, -294.3), vec3(15, 1, 117.1603));
	CreateCube(vec3(-550.5, 69.6, -369.3), vec3(15, 1, 39.27095), -18.2262, vec3(0, 1, 0));//48
	CreateCube(vec3(-518.5, 69.6, -369.3), vec3(15, 1, 39.27095), 18.2262, vec3(0, 1, 0));//49
	CreateCube(vec3(-541.5, 69.6, -392.3), vec3(15, 1, 17.55126), -31.3407, vec3(0, 1, 0));//50
	//50
	CreateCube(vec3(-527.5, 69.6, -392.3), vec3(15, 1, 17.55126), 31.3407, vec3(0, 1, 0));
	CreateCube(vec3(-534.5, 69.6, -412.3), vec3(20, 1, 31.76958));
	CreateCube(vec3(-534.5, 42.6, -469.3), vec3(7.5, 1, 240.2195));
	CreateCube(vec3(-534.5, 70.39, -435.3), vec3(20, 1, 15.75972), 6.138, vec3(1, 0, 0));
	CreateCube(vec3(-534.5, 72.6, -450.3), vec3(20, 1, 15.75972), 10.67, vec3(1, 0, 0));
	//55
	CreateCube(vec3(-534.5, 75.91, -465.3), vec3(20, 1, 15.75972), 14.54, vec3(1, 0, 0));

	CreateCube(vec3(-534.5, 72.6, -589.3), vec3(46, 60, 1), 0.0f, vec3(0, 0, 0), true);//4t checkpoint

	//Sphere
	CreateSphere(vec3(-213.5, 42.6, 203.7), 13);
	CreateSphere(vec3(-284.5, 42.6, 163.7), 13);
	CreateSphere(vec3(-337.5, 42.6, 188.7), 13);
	CreateSphere(vec3(-407.5, 42.6, 167.7), 13);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (p2List_item<Primitive*>* tmp = primitives.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		delete tmp->data;
	}
	primitives.clear();

	for (p2List_item<Cube*>* tmp = s.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		delete tmp->data;
	}
	s.clear();

	/*for (p2List_item<PhysBody3D*>* tmp = sensor.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		delete tmp->data;//Ja es netegen els collisionObjects
	}*/
	sensor.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane floor(0, 1, 0, 0);
	if (App->player->debug)
	{
		floor_cube.Render();
		floor.axis = true;
	}

	floor.color.Set(255, 0, 0);
	//-----------------
	mat4x4 player_pos;
	App->player->vehicle->GetTransform(&player_pos);
	floor.SetPos(player_pos.M[12], 0, player_pos.M[14]);
	//-----------------
	floor.Render();

	Cube* tmp;
	if(s.at(App->player->checkpoint, tmp))
		s.findNode(tmp)->data->Render();
	else
	{
		LOG("Win!!");
		App->player->checkpoint = -1;
		App->player->reset = true;
	}

	//-----------------
	for (p2List_item<Primitive*>* tmp = primitives.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		tmp->data->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("Hit!");

	if (body1 == floor_sensor && body2 == App->player->vehicle)
	{
		App->player->reset = true;
	}

	PhysBody3D* tmp;
	sensor.at((App->player->checkpoint), tmp);
	if (body1 == tmp && body2 == App->player->vehicle)
	{
		App->player->checkpoint++;
		App->player->vehicle->GetTransform(&App->player->last_checkpoint_matrix);
	}
}

void ModuleSceneIntro::CreateCube(const vec3& position, const vec3& size, float angle, const vec3& rotAxis, bool is_sensor)
{
	Cube* c = new Cube();
	c->size.Set(size.x, size.y, size.z);
	c->SetPos(position.x, position.y, position.z);
	if (angle != 0.0f)
		c->SetRotation(angle, rotAxis);

	if (is_sensor)
	{
		c->color.Set(0, 255, 0, 0.5f);
		s.add(c);
		sensor.add(App->physics->AddBody(*c, 0.0f));
		sensor.getLast()->data->SetAsSensor(true);
		sensor.getLast()->data->collision_listeners.add(this);
	}
	else
	{
		primitives.add(c);
		App->physics->AddBody(*c, 0);
	}

}

void ModuleSceneIntro::CreateSphere(const vec3& position, float radius)
{
	Sphere* c = new Sphere();
	c->radius = radius;
	c->SetPos(position.x, position.y, position.z);

	App->physics->AddBody(*c, 0);

	primitives.add(c);
}
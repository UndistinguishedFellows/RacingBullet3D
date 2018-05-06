#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * 15);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
	{
		LOG("SDL_JOYSTICK could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	jaxis = accel = deaccel = 0;

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	SDL_Joystick* joystick;
	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_JOYAXISMOTION:
				
					if (e.jaxis.axis == 0)
					{
						if ((e.jaxis.value < -10000) || (e.jaxis.value > 10000))
						{
							if (e.jaxis.value < 0)
							{
								jaxis = (float)e.jaxis.value / 32768;
							}

							if (e.jaxis.value > 0)
							{
								jaxis = (float)e.jaxis.value / 32767;
							}
						}
						else
							jaxis = 0;
					}

					if (e.jaxis.axis == 1)
					{
						/* Up-Down movement code goes here */
					}

					if (e.jaxis.axis == 2)
					{
						//mouse_x_motion = (int)e.jaxis.value / 320;
					}

					if (e.jaxis.axis == 5)
					{
						if (e.jaxis.value > -32000)
							accel = ((float)e.jaxis.value + 32768) / 32767 / 2;
						else
							accel = 0;
					}

					if (e.jaxis.axis == 4)
					{
						if (e.jaxis.value > -32000)
							deaccel = ((float)e.jaxis.value + 32768) / 32767 / 2;
						else
							deaccel = 0;
					}
				break;

			case SDL_JOYBUTTONDOWN:
				for (int x = 0; x < 14; x++)
				{
					if (e.jbutton.button == x)
					{
						j_buttons[x] = KEY_DOWN;
					}
				}
				break;

			case SDL_JOYBUTTONUP:
				for (int x = 0; x < 14; x++)
				{
					if (e.jbutton.button == x)
					{
						j_buttons[x] = KEY_UP;
					}
				}
				break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	return true;
}
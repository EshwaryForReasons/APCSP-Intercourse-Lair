
#include <iostream>
#include <string>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "main.h"
#include "ILObject.h"
#include "Player.h"
#include "Resources.h"
#include "World.h"
#include "Worlds/BasicWorld.h"

static Main* global_main = nullptr;

static std::string ErrorList = {};

void InsertNewError(std::string Error)
{
	ErrorList.append(Error);

	SI_LOG(Error);
}


Main::Main()
{
	global_main = this;

	b_has_started = false;

	quit = false;
}

Main::~Main()
{
	global_main = nullptr;
	quit = true;
}

Main* Main::GetInstance()
{
	return global_main;
}


void Main::MainTick()
{
	//Start physics engine
	ILPhysics();

	StartGame();

	SDL_Event event;

	uint64_t start_time = 0;
	uint64_t end_time = 0;

	while (!quit)
	{
		float seconds_elapsed = (end_time - start_time) / 1000.0f;

		start_time = SDL_GetTicks();

		//Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			if (!b_has_started && event.key.keysym.sym == SDLK_SPACE)
			{
				b_has_started = true;
				StartGame();
			}

			if (current_world)
			{
				current_world->HandleInput(event);
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, 0x35, 0x47, 0x48, 0xFF);
		SDL_RenderClear(Main::GetInstance()->global_renderer);

		if (current_world)
		{
			current_world->RenderWorld(seconds_elapsed);
		}

		//Update screen
		SDL_RenderPresent(Main::GetInstance()->global_renderer);

		end_time = SDL_GetTicks();
	}
}


void Main::StartGame()
{
	delete current_world;
	current_world = nullptr;
	current_world = new ILBasicWorld();
}


bool Main::InitializeSDL()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0)
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			SI_LOG("Warning: Linear texture filtering not enabled.")
		}

		//Create window
		global_window = SDL_CreateWindow("Intercourse Lair", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (global_window)
		{
			//Create vsynced renderer for window
			global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED);

			if (!global_renderer)
			{
				SI_LOG("Renderer could not be created. SDL Error: " << SDL_GetError())

				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(global_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int image_flags = IMG_INIT_PNG;

				if (!(IMG_Init(image_flags) & image_flags))
				{
					SI_LOG("SDL_image could not be initialized. SDL_image Error: " << IMG_GetError())

					success = false;
				}

				if (TTF_Init() == -1)
				{
					SI_LOG("SDL_ttf could not initialize. SDL_ttf Error: " << TTF_GetError())

					success = false;
				}
			}
		}
		else
		{
			SI_LOG("Window could not be created. SDL Error: " << SDL_GetError())

			success = false;
		}
	}
	else
	{
		SI_LOG("SDL could not initialize. SDL Error: " << SDL_GetError())

		success = false;
	}

	return success;
}

void Main::Close()
{
	//Free loaded images
	Resources::FreeMedia();

	//Destroy window	
	SDL_DestroyRenderer(global_renderer);
	SDL_DestroyWindow(global_window);
	global_window = NULL;
	global_renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int WinMain(int argc, char* argv[])
{
#if _DEBUG
	InitializeLogFile();
#endif

	//Start the main game
	Main main_app;

	//Start up SDL and create window
	if (Main::GetInstance()->InitializeSDL())
	{
		//Load media
		if (Resources::LoadMedia())
		{
			Main::GetInstance()->MainTick();
		}
		else
		{
			SI_LOG("Failed to load media.")
		}
	}
	else
	{
		SI_LOG("Failed to initialze.")
	}

	//Free resources and close SDL
	Main::GetInstance()->Close();

	return 0;
}
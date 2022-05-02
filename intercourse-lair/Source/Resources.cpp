
#include "Resources.h"
#include "main.h"

bool Resources::LoadMedia()
{
	bool success = true;

	global_font_title = TTF_OpenFont("space_invaders.ttf", 28);
	global_font_normal = TTF_OpenFont("space_invaders.ttf", 20);
	global_font_small = TTF_OpenFont("space_invaders.ttf", 14);

	//Load background
	if (!background_texture.LoadFromFile("background_texture.png"))
	{
		SI_LOG("Failed to load the player texture.")

		success = false;
	}

	//Load projectile bullet texture
	if (!ground_texture.LoadFromFile("ground.png"))
	{
		SI_LOG("Failed to load the ground texture.")

		success = false;
	}

	AnimationList player_animations;
	player_animations.emplace("idle", new ILAnimation(10, 0, 0.5));
	player_animations.emplace("run", new ILAnimation(10, 1, 0.5));
	player_animations.emplace("attack", new ILAnimation(4, 2, 0.5));
	player_animations.emplace("jump", new ILAnimation(10, 3, 0.5));
	player_animations.emplace("death", new ILAnimation(10, 4, 2));

	player_texture = ILTexture(player_animations);

	//Load player texture
	if (!player_texture.LoadFromFile("player.png"))
	{
		SI_LOG("Failed to load the player texture.")

		success = false;
	}

	AnimationList death_animations;
	death_animations.emplace("idle", new ILAnimation(16, 0, 0.5));
	death_animations.emplace("run", new ILAnimation(16, 1, 0.5));
	death_animations.emplace("attack", new ILAnimation(8, 2, 0.5));
	death_animations.emplace("death", new ILAnimation(8, 3, 2));

	death_texture = ILTexture(death_animations, 2.0f);

	//Load player texture
	if (!death_texture.LoadFromFile("death_texture.png"))
	{
		SI_LOG("Failed to load the enemy texture.")

		success = false;
	}

	return success;
}

void Resources::FreeMedia()
{
	background_texture.Free();
	player_texture.Free();
	death_texture.Free();
}
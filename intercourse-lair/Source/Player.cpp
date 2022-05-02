//Copyright Eshwary Mishra 2022

#include <SDL.h>

#include "Player.h"
#include "main.h"
#include "SDLUtilities/Texture.h"
#include "Resources.h"
#include "World.h"


ILPlayer::ILPlayer(int x, int y, ILTexture* _pawn_texture) : ILPawn(x, y, _pawn_texture)
{
	health = 100;
	max_health = 100;

	InitializePlayerHUD();
}

void ILPlayer::Tick(float delta_time)
{
	ILPawn::Tick(delta_time);

	CreatePlayerHUD();
}


void ILPlayer::HandleInput(SDL_Event& input_event)
{
	if (b_is_dead)
		return;

	const uint8_t* k = SDL_GetKeyboardState(NULL);

	//If a key was pressed
	if (input_event.type == SDL_KEYDOWN && input_event.key.repeat == 0)
	{
		switch (input_event.key.keysym.sym)
		{
		case SDLK_a: object_movement_information.x_acceleration = -object_movement_information.max_x_velocity; break;
		case SDLK_d: object_movement_information.x_acceleration = object_movement_information.max_x_velocity; break;
		case SDLK_w: Jump(); break;
		case SDLK_SPACE: Attack(); break;
		case SDLK_u: UpgradeBullet(); break;
		case SDLK_l: BuyLife(); break;
		}
	}

	//If a key was released
	else if (input_event.type == SDL_KEYUP && input_event.key.repeat == 0)
	{
		if (!k[SDL_SCANCODE_A] && !k[SDL_SCANCODE_D]) {object_movement_information.x_acceleration = 0;}
		else if (k[SDL_SCANCODE_A] && !k[SDL_SCANCODE_D]) {object_movement_information.x_acceleration = -object_movement_information.max_x_velocity;}
		else if (!k[SDL_SCANCODE_A] && k[SDL_SCANCODE_D]) {object_movement_information.x_acceleration = object_movement_information.max_x_velocity;}
	}
}


void ILPlayer::Jump()
{
	if (object_movement_information.b_has_landed)
	{
		object_movement_information.y_acceleration = -50;
		object_texture.SetAnimation("jump");
	}
}

void ILPlayer::Attack()
{
	b_is_attacking = true;
	object_texture.SetAnimation("attack", &b_is_attacking);
	object_movement_information.x_acceleration = 0;

	//Deal damage
	std::vector<ILObject*> hit_objects = Main::GetWorld()->CheckMultipleGlobalCollision(GetSimpleCollider(), false);

	for (ILObject* x : hit_objects)
	{
		x->TakeDamage(20.0f);
	}
}


void ILPlayer::UpgradeBullet()
{
	/*if (bullet_level == 0 && Main::GetInstance()->current_game_mode->player_money >= 500)
	{
		Main::GetInstance()->current_game_mode->player_money -= 500;
		bullet_level = 1;
		bullet_upgrade_texture.LoadFromRenderedText("U - UPGRADE BULLET: $1350", {255, 255, 255}, EFontSize::FS_SMALL);
	}
	else if (bullet_level == 1 && Main::GetInstance()->current_game_mode->player_money >= 1350)
	{
		Main::GetInstance()->current_game_mode->player_money -= 1350;
		bullet_level = 2;
		bullet_upgrade_texture.LoadFromRenderedText("U - UPGRADE BULLET: $9000", {255, 255, 255}, EFontSize::FS_SMALL);
	}
	else if (bullet_level == 2 && Main::GetInstance()->current_game_mode->player_money >= 9000)
	{
		Main::GetInstance()->current_game_mode->player_money -= 9000;
		bullet_level = 3;
		bullet_upgrade_texture.LoadFromRenderedText("U - UPGRADE BULLET: MAX", {255, 255, 255}, EFontSize::FS_SMALL);
	}*/
}

void ILPlayer::BuyLife()
{
	/*if (health < 3 && Main::GetInstance()->current_game_mode->player_money >= 1500)
	{
		health++;
	}*/
}


void ILPlayer::SpawnProjectile()
{
	//AProjectileBullet* projectile = new AProjectileBullet(object_movement_information.location.x + object_width / 2, object_movement_information.location.y - 10, &Resources::projectile_bullet_texture, this, bullet_level);

	//Main::GetInstance()->AddEntity(projectile);
}


void ILPlayer::InitializePlayerHUD()
{
	background_color = color(0, 0, 0, 255);
	foreground_color = color(255, 0, 0, 255);
}

void ILPlayer::CreatePlayerHUD()
{
	const int x = 20;
	const int y = 20;
	const int w = max_health * 2;
	const int h = 10;

	float Percent = clamp(health / max_health, 0.0f, 1.0f);
	SDL_GetRenderDrawColor(Main::GetInstance()->global_renderer, &old.r, &old.g, &old.g, &old.a);
	background_rect = {x, y, w, h};
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
	SDL_RenderFillRect(Main::GetInstance()->global_renderer, &background_rect);
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, foreground_color.r, foreground_color.g, foreground_color.b, foreground_color.a);
	int pw = (int)((float)w * Percent);
	int px = x + (w - pw);
	foreground_rect = {px, y, pw, h};
	SDL_RenderFillRect(Main::GetInstance()->global_renderer, &foreground_rect);
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, old.r, old.g, old.b, old.a);
}
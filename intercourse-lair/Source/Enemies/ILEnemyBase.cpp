
#include "ILEnemyBase.h"

#include "main.h"
#include "World.h"
#include "Player.h"

ILEnemyBase::ILEnemyBase(int x, int y, int current_level, ILTexture* pawn_texture) : ILPawn(x, y, pawn_texture)
{
	max_health = 200.0f * current_level * 0.45f;
	health = max_health;
	last_attack_time = 0;
	time_between_attacks = clamp(3000 - (300 * current_level), 250, 3000);
	attack_damage = 20 * current_level * 0.45;

	object_movement_information.max_x_velocity = 200;

	damage_zone.x = object_movement_information.location.x;
	damage_zone.y = object_movement_information.location.y;
	damage_zone.w = object_texture.GetFrameWidth() * 1.5;
	damage_zone.h = object_texture.GetFrameHeight();
}

void ILEnemyBase::Tick(float delta_time)
{
	ILPawn::Tick(delta_time);

	if (!b_is_dead)
	{
		if (Main::GetWorld()->player->object_movement_information.location.x > object_movement_information.location.x)
			object_movement_information.x_acceleration = object_movement_information.max_x_velocity;
		else if (Main::GetWorld()->player->object_movement_information.location.x < object_movement_information.location.x)
			object_movement_information.x_acceleration = -object_movement_information.max_x_velocity;

		if (last_attack_time <= 0)
			Attack();
		else
			last_attack_time -= time_between_attacks * delta_time;
	}
	else
		object_movement_information.x_acceleration = 0;

	ShowHealthBar();
}

void ILEnemyBase::Die()
{
	ILPawn::Die();

	if (Main::GetWorld()->player)
		Main::GetWorld()->player->health = Main::GetWorld()->player->max_health;

	Main::GetWorld()->ProgressLevel();
}


void ILEnemyBase::Attack()
{
	last_attack_time = time_between_attacks;

	b_is_attacking = true;
	object_texture.SetAnimation("attack", &b_is_attacking);
	object_movement_information.x_acceleration = 0;

	damage_zone.x = object_movement_information.location.x;
	damage_zone.y = object_movement_information.location.y;
	damage_zone.w = object_texture.GetFrameWidth() * 1.5;
	damage_zone.h = object_texture.GetFrameHeight();

	SDL_RenderFillRect(Main::GetInstance()->global_renderer, &damage_zone);

	//Deal damage
	std::vector<ILObject*> hit_objects = Main::GetWorld()->CheckMultipleGlobalCollision(&damage_zone, true);

	for (ILObject* x : hit_objects)
	{
		if(x == (ILObject*)Main::GetWorld()->player)
			x->TakeDamage(20.0f);
	}
}


void ILEnemyBase::ShowHealthBar()
{
	const SDL_Color background_color = color(0, 0, 0, 255);;
	const SDL_Color foreground_color = color(255, 0, 0, 255);;
	SDL_Color old;

	const int x = object_movement_information.location.x + object_texture.GetFrameWidth() - 90.0f;
	const int y = object_movement_information.location.y;
	const int w = 80.0f;
	const int h = 10;

	float Percent = clamp(health / max_health, 0.0f, 1.0f);
	SDL_GetRenderDrawColor(Main::GetInstance()->global_renderer, &old.r, &old.g, &old.g, &old.a);
	const SDL_Rect background_rect = {x, y, w, h};
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
	SDL_RenderFillRect(Main::GetInstance()->global_renderer, &background_rect);
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, foreground_color.r, foreground_color.g, foreground_color.b, foreground_color.a);
	int pw = (int)((float)w * Percent);
	int px = x + (w - pw);
	const SDL_Rect foreground_rect = {px, y, pw, h};
	SDL_RenderFillRect(Main::GetInstance()->global_renderer, &foreground_rect);
	SDL_SetRenderDrawColor(Main::GetInstance()->global_renderer, old.r, old.g, old.b, old.a);
}
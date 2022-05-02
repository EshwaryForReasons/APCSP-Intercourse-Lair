
#include <iostream>

#include "Pawn.h"

#include "Player.h"
#include "main.h"
#include "World.h"

ILPawn::ILPawn(int x, int y, ILTexture* _pawn_texture) : ILObject(x, y, _pawn_texture)
{
	health = 100;
	max_health = 100;
	b_is_dead = false;

	b_is_attacking = false;
	b_is_death_animation_playing = false;
}

void ILPawn::Tick(float delta_time)
{
	ILObject::Tick(delta_time);

	if (object_movement_information.x_acceleration < 0)
		flip = SDL_FLIP_HORIZONTAL;
	else if (object_movement_information.x_acceleration > 0)
		flip = SDL_FLIP_NONE;

	if (!b_is_attacking && !b_is_dead && object_movement_information.b_has_landed)
	{
		if (object_movement_information.x_acceleration != 0)
			object_texture.SetAnimation("run");
		if (object_movement_information.x_acceleration == 0)
			object_texture.SetAnimation("idle");
	}
	else if (!object_movement_information.b_has_landed)
	{
		b_is_attacking = false;
		object_texture.SetAnimation("jump");
	}

	if (b_is_dead && !b_is_death_animation_playing)
	{
		FinishDeath();
	}
}


void ILPawn::TakeDamage(float damage)
{
	if (!b_is_dead)
	{
		ILObject::TakeDamage(damage);

		//Update health
		health = clamp(health - damage, 0.0f, max_health);

		//Die if no health is left
		if (health <= 0.0f)
		{
			Die();
		}
	}
}

void ILPawn::Die()
{
	ILObject::Die();

	b_is_death_animation_playing = true;
	b_is_dead = true;
	object_texture.SetAnimation("death", &b_is_death_animation_playing);

	object_movement_information.x_acceleration = 0;
}

void ILPawn::FinishDeath()
{
	Main::GetWorld()->RemoveEntity(this);
}
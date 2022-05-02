
#include "ILObject.h"
#include "SDLUtilities/Texture.h"
#include "ILPhysics.h"
#include "main.h"
#include "World.h"

ILObject::ILObject(int x, int y, ILTexture* _object_texture)
{
	object_texture = *_object_texture;

	object_movement_information.x_acceleration = 0;
	object_movement_information.y_acceleration = 0;
	object_movement_information.x_deceleration = 1500;
	object_movement_information.y_deceleration = 1500;

	object_movement_information.b_has_landed = false;

	//Initialize the offsets
	object_movement_information.location.x = x;
	object_movement_information.location.y = y;

	//Initialize the velocity
	object_movement_information.x_velocity = 0;
	object_movement_information.y_velocity = 0;
	object_movement_information.max_x_velocity = 500;
	object_movement_information.max_y_velocity = 1000;

	object_width = object_texture.GetFrameWidth();
	object_height = object_texture.GetFrameHeight();

	simple_collider.x = object_movement_information.location.x;
	simple_collider.y = object_movement_information.location.y;
	simple_collider.w = object_texture.GetFrameWidth();
	simple_collider.h = object_texture.GetFrameHeight();

	landed_collider.x = object_movement_information.location.x;
	landed_collider.y = object_movement_information.location.y;
	landed_collider.w = object_texture.GetFrameWidth();
	landed_collider.h = object_texture.GetFrameHeight() + 2;

	flip = SDL_FLIP_NONE;
}

ILObject::~ILObject()
{
	object_texture.Free();
}

void ILObject::Tick(float delta_time)
{
	
}

void ILObject::Render(float delta_time)
{
	//Apply the physics and find new location
	if (!object_movement_information.b_has_landed)
	{
		Vector gravity = ILPhysics::ApplyGravityToObject(object_mass);
		object_movement_information.y_acceleration -= gravity.y * delta_time;
	}
	
	MoveObject(delta_time);
	
	//Show the texture
	object_texture.Render(delta_time, object_movement_information.location.x, object_movement_information.location.y, flip);

	Tick(delta_time);
}


bool ILObject::MoveObject(float delta_time)
{
	object_movement_information.x_velocity = clamp(object_movement_information.x_velocity + object_movement_information.x_acceleration, -object_movement_information.max_x_velocity, object_movement_information.max_x_velocity);
	object_movement_information.location.x += object_movement_information.x_velocity * delta_time;
	RecalculateCollision();

	if (object_movement_information.x_acceleration == 0)
		object_movement_information.x_velocity = 0;

	if (Main::GetWorld()->CheckGlobalWallCollision(&simple_collider))
	{
		//Move back
		object_movement_information.location.x -= object_movement_information.x_velocity * delta_time;
		simple_collider.x = object_movement_information.location.x;

		object_movement_information.x_acceleration = 0;

		return false;
	}

	object_movement_information.y_velocity = clamp(object_movement_information.y_velocity + object_movement_information.y_acceleration, -object_movement_information.max_y_velocity, object_movement_information.max_y_velocity);
	object_movement_information.location.y += object_movement_information.y_velocity * delta_time;
	RecalculateCollision();

	if (Main::GetWorld()->CheckGlobalWallCollision(&simple_collider))
	{
		//Move back
		object_movement_information.location.y -= object_movement_information.y_velocity * delta_time;
		simple_collider.y = object_movement_information.location.y;

		object_movement_information.b_has_landed = true;
		object_movement_information.y_acceleration = 0;
		object_movement_information.y_velocity = 0;

		return false;
	}

	//Check if we are on a surface
	object_movement_information.b_has_landed = Main::GetWorld()->CheckGlobalWallCollision(&landed_collider);

	return true;
}

void ILObject::RecalculateCollision()
{
	object_width = object_texture.GetFrameWidth();
	object_height = object_texture.GetFrameHeight();

	simple_collider.x = object_movement_information.location.x;
	simple_collider.y = object_movement_information.location.y;
	simple_collider.w = object_texture.GetFrameWidth();
	simple_collider.h = object_texture.GetFrameHeight();

	landed_collider.x = object_movement_information.location.x;
	landed_collider.y = object_movement_information.location.y;
	landed_collider.w = object_texture.GetFrameWidth();
	landed_collider.h = object_texture.GetFrameHeight() + 2;
}


void ILObject::TakeDamage(float damage)
{
	
}

void ILObject::Die()
{
	
}
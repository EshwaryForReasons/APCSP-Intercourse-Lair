
#include "ILPhysics.h"
#include "main.h"

ILPhysics::ILPhysics()
{
	gravity_amount = 9.8;
}


Vector ILPhysics::ApplyGravityToObject(int object_mass)
{
	Vector final_velocity = {0, 0};

	//Apply gravity
	final_velocity.y -= gravity_amount * object_mass;

	return final_velocity;
}
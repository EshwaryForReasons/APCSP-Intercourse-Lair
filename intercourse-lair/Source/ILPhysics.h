#pragma once

/**The coordinate location in 2D world space*/
struct Vector
{
	float x;
	float y;
};

struct ObjectMovementInformation
{
	float max_x_velocity;
	float max_y_velocity;

	float x_velocity;
	float y_velocity;

	float x_acceleration;
	float y_acceleration;
	float x_deceleration;
	float y_deceleration;

	bool b_has_landed;

	Vector location;
};

class ILPhysics
{
public:

	ILPhysics();


	/**Calculate the velocity after applying gravity*/
	static Vector ApplyGravityToObject(int object_mass);

	/**Set the gravity*/
	static void SetGravityScale(float gravity_multiplier) {gravity_amount = 9.8 * gravity_multiplier;}

protected:

	/**The amount of gravity in m/s*/
	inline static float gravity_amount;
};

#pragma once

#include "Source/Pawn.h"

class ILEnemyBase : public ILPawn
{
public:

	ILEnemyBase(int x, int y, int current_level, ILTexture* pawn_texture);

	/**Called every frame*/
	virtual void Tick(float delta_time) override;


	/**The death logic*/
	virtual void Die() override;

	virtual void Attack();

protected:

	/**The damage zone of this enemy*/
	SDL_Rect damage_zone;

	/**Time since last attack*/
	float last_attack_time;

	/**Time between attacks*/
	float time_between_attacks;

	/**Attack damage*/
	float attack_damage;


	/**Show health bar above the enemy*/
	void ShowHealthBar();
};


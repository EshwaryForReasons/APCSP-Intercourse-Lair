
#pragma once

#include <vector>

#include "SDLUtilities/Texture.h"
#include "ILObject.h"

class ILPawn : public ILObject
{
public:

	ILPawn(int x, int y, ILTexture* pawn_texture);

	/**Called every frame*/
	virtual void Tick(float delta_time) override;


	/**Take damage*/
	virtual void TakeDamage(float damage) override;

	/**The death logic*/
	virtual void Die() override;


	/**This pawn's health*/
	float health;

	/**The pawn's max health*/
	float max_health;

	/**Are we dead*/
	bool b_is_dead;

protected:

	/**Finish death logic*/
	void FinishDeath();


	/**Are we currently attacking*/
	bool b_is_attacking;

	/**Is the dead animation finished*/
	bool b_is_death_animation_playing;
};


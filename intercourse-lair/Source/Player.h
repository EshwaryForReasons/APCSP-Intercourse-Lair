
#pragma once

#include <vector>

#include "pawn.h"

class ILPlayer : public ILPawn
{
public:

	ILPlayer(int x, int y, ILTexture* pawn_texture);

	/**Called every frame*/
	virtual void Tick(float delta_time) override;

	/**Handlers user input*/
	virtual void HandleInput(SDL_Event& input_event);

protected:

	SDL_Color background_color;
	SDL_Color foreground_color;
	SDL_Color old;
	SDL_Rect background_rect;
	SDL_Rect foreground_rect;

	/**Handles jumping*/
	void Jump();

	/**Handles attacking*/
	void Attack();


	/**Spawns a projectile*/
	void SpawnProjectile();

	/**Initialized the UI elements for the HUD*/
	void InitializePlayerHUD();

	/**Creates the player HUD*/
	void CreatePlayerHUD();

	/**Upgrades bullet*/
	void UpgradeBullet();

	/**Buys a life*/
	void BuyLife();
};
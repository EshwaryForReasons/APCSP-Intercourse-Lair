
#include "BasicWorld.h"
#include "main.h"
#include "ILObject.h"
#include "Player.h"
#include "Enemies/ILEnemyBase.h"
#include "Resources.h"
#include "Wall.h"

ILBasicWorld::ILBasicWorld()
{
	ILWorld();

	if (Main::GetInstance()->b_has_started)
	{
		//Spawn player
		player = new ILPlayer(400, 200, &Resources::player_texture);
		AddEntity(player);

		//Spawn enemy
		AddEntity(new ILEnemyBase(200, 200, 1, &Resources::death_texture));

		//Add a floor
		AddWall(new ILWall(0, SCREEN_HEIGHT - 150, 1000, 150, &Resources::ground_texture));

		game_over_text.LoadFromRenderedText("YOU DIED", { 255, 255, 255 }, EFontSize::FS_TITLE);
		restart_text.LoadFromRenderedText("PRESS SPACE TO RESTART", { 255, 255, 255 }, EFontSize::FS_NORMAL);
	}
	
	title_text.LoadFromRenderedText("INTERCOURSE LAYER", {255, 255, 255}, EFontSize::FS_TITLE);
	start_text.LoadFromRenderedText("PRESS SPACE TO START", {255, 255, 255}, EFontSize::FS_NORMAL);
}

ILBasicWorld::~ILBasicWorld()
{
	ILWorld::~ILWorld();
}

void ILBasicWorld::RenderWorld(float delta_time)
{
	if (Main::GetInstance()->b_has_started)
	{
		Resources::background_texture.Render(-100, 0, Resources::background_texture.GetWidth() * 1.4, SCREEN_HEIGHT);

		ILWorld::RenderWorld(delta_time);

		if (!player || player->b_is_dead)
		{
			game_over_text.Render((SCREEN_WIDTH / 2) - 100, 200, 200, 40);
			restart_text.Render((SCREEN_WIDTH / 2) - 100, 400, 200, 40);
		}
	}
	else
	{
		title_text.Render((SCREEN_WIDTH / 2) - 300, 200, 600, 80);
		start_text.Render((SCREEN_WIDTH / 2) - 100, 400, 200, 40);
	}
}
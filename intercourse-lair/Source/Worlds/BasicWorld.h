#pragma once

#include "World.h"
#include "SDLUtilities/Texture.h"

class ILBasicWorld : public ILWorld
{
public:

	ILBasicWorld();
	~ILBasicWorld();

	/**Called every frame, responsible for rendering all the world objects*/
	virtual void RenderWorld(float delta_time) override;

private:

	class ILTexture game_over_text;
	class ILTexture restart_text;
	class ILTexture title_text;
	class ILTexture start_text;
};
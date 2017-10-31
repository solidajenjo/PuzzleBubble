#pragma once
#include "Sprite.h"
#include "TileMap.h"

class Glow
{
public:
	void init(ShaderProgram &shaderProgram, glm::vec2 &position, int color);
	void update(int deltaTime);
	void render();
	int getState();
	Glow();
	~Glow();
private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int state;
};


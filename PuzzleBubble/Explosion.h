#pragma once
#include "Sprite.h"
#include "TileMap.h"

class Explosion
{
public:
	void init(ShaderProgram &shaderProgram, glm::vec2 &position, int color);
	void update(int deltaTime);
	void render();
	int getState();
	Explosion();
	~Explosion();
private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int state;
};


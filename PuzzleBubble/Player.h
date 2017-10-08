#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Ball.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 position);
	void update(int deltaTime, Ball *currentBall);
	glm::vec2 getArrowDirection();
	bool isBallShot();
	void ballShotAcquired();
	void setWaitingTime(int time);
	void render();
	
	
private:
	bool ballShot = false;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, waitingToShoot = 0;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float arrowAngle = 0.;
	glm::vec2 position;
	glm::vec4 arrowDirection = glm::vec4(0.f, 1.f, 0.f, 1.f);
};


#endif // _PLAYER_INCLUDE



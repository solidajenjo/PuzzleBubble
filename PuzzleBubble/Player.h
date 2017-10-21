#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Ball.h"


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 position);
	void update(int deltaTime, Ball *currentBall);
	glm::vec2 getArrowDirection();
	bool isBallShot();
	void ballShotAcquired();
	void setWaitingTime(int time);
	void setBlocked(bool status);
	void render();
	bool anyKeyPressed();
	
private:
	bool ballShot = false;
	bool blocked = false;
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



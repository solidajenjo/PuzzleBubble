#include <cmath>
#include <iostream>
#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include "dependencies\freeglut\include\GL\glut.h"
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Player.h"
#include "Game.h"

#define MAX_ARROW_ROT 1.1f
#define PLAYER_CENTER_X 27 //Axis of arrow 
#define PLAYER_CENTER_Y 52
#define ROT_SPEED 0.002f
#define WAIT_TIME 1000

enum moves {
	STILL, RIGHT, LEFT
};

void Player::init(TileMap *map, ShaderProgram &shaderProgram, glm::vec2 position)
{
	spritesheet.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(55.f,89.f), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setSpriteCenter(glm::vec2(PLAYER_CENTER_X, PLAYER_CENTER_Y));
	sprite->setPosition(glm::vec2(position.x - PLAYER_CENTER_X, position.y - PLAYER_CENTER_Y));
	this->position = position;
	this->map = map;
	arrowDirection = -glm::normalize(glm::rotate(glm::mat4(1.0f), arrowAngle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 1.f));

	//bub's sprites

	bubsheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bubsheet.setWrapS(GL_CLAMP_TO_EDGE);
	bubsheet.setWrapT(GL_CLAMP_TO_EDGE);
	bubsheet.setMinFilter(GL_NEAREST);
	bubsheet.setMagFilter(GL_NEAREST);
	bub = Sprite::createSprite(glm::vec2(70.f, 34.f), glm::vec2(0.125f, 1.f), &bubsheet, &shaderProgram);
	bub->setNumberAnimations(3);
	bub->setAnimationSpeed(STILL, 1);
	bub->addKeyframe(STILL, glm::vec2(0.f, 0.f));
	bub->setAnimationSpeed(RIGHT, 30);
	bub->addKeyframe(RIGHT, glm::vec2(0.f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.125f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.25f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.375f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.5f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.625f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.75f, 0.f));
	bub->addKeyframe(RIGHT, glm::vec2(0.875f, 0.f));
	bub->setAnimationSpeed(LEFT, 30);
	bub->addKeyframe(LEFT, glm::vec2(0.f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.875f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.75f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.625f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.5f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.375f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.25f, 0.f));
	bub->addKeyframe(LEFT, glm::vec2(0.125f, 0.f));
	bub->changeAnimation(STILL);
	bub->setPosition(glm::vec2(position.x + 5, position.y));
}

void Player::update(int deltaTime, Ball *currentBall)
{	
	if (waitingToShoot > 0) waitingToShoot -= deltaTime;
	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && arrowAngle < MAX_ARROW_ROT)
	{
		if (bub->animation() != RIGHT) bub->changeAnimation(RIGHT);
		arrowAngle += ROT_SPEED * deltaTime;
		arrowDirection = -glm::normalize(glm::rotate(glm::mat4(1.0f), arrowAngle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 1.f));
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && arrowAngle > -MAX_ARROW_ROT)
	{
		if (bub->animation() != LEFT) bub->changeAnimation(LEFT);
		arrowAngle -= ROT_SPEED * deltaTime;
		arrowDirection = -glm::normalize(glm::rotate(glm::mat4(1.0f), arrowAngle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 1.f));
	}
	else if (currentBall != NULL && Game::instance().getKey(32) && waitingToShoot <= 0 && !map->isUpdating()) {
		waitingToShoot += WAIT_TIME;
		ballShot = true;
		currentBall->setDirection(glm::vec2(arrowDirection));
	}
	else bub->changeAnimation(STILL);
	bub->update(deltaTime);
	sprite->rotate(arrowAngle);
	sprite->setPosition(position);
}

glm::vec2 Player::getArrowDirection()
{
	return glm::vec2(arrowDirection);
}

bool Player::isBallShot()
{
	return ballShot;
}

void Player::ballShotAcquired()
{
	ballShot = false;
}

void Player::setWaitingTime(int time)
{
	waitingToShoot = time;
}


void Player::render()
{
	sprite->render();
	bub->render();
}

bool Player::anyKeyPressed()
{
	for (int i = 0; i < 256; ++i) {
		if (Game::instance().getKey(i)) return true;
		if (Game::instance().getSpecialKey(i)) return true;
	}
	return false;
}






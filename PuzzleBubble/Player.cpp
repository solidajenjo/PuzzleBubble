#include <cmath>
#include <iostream>
#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include "dependencies\freeglut\include\GL\glut.h"
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Player.h"
#include "Game.h"

#define MAX_ARROW_ROT 1.1f
#define PLAYER_CENTER_CORRECTION_X 1 //Correction for arrow rotation purposes
#define PLAYER_CENTER_CORRECTION_Y 4
#define ROT_SPEED 0.005f
#define WAIT_TIME 2000

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 position)
{
	spritesheet.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(102.f,102.f), glm::vec2(PLAYER_CENTER_CORRECTION_X, PLAYER_CENTER_CORRECTION_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setPosition(position);
	this->position = position;
}

void Player::update(int deltaTime, Ball *currentBall)
{	
	if (waitingToShoot > 0) waitingToShoot -= deltaTime;
	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (arrowAngle < MAX_ARROW_ROT) arrowAngle += ROT_SPEED * deltaTime;
		arrowDirection = -glm::normalize(glm::rotate(glm::mat4(1.0f), arrowAngle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 1.f));
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (arrowAngle > -MAX_ARROW_ROT) arrowAngle -= ROT_SPEED * deltaTime;
		arrowDirection = -glm::normalize(glm::rotate(glm::mat4(1.0f), arrowAngle, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(0.f, 1.f, 0.f, 1.f));
	}
	else if (Game::instance().getKey(32) && waitingToShoot <= 0) {
		waitingToShoot += WAIT_TIME;
		ballShot = true;
		currentBall->setDirection(glm::vec2(arrowDirection));
	}
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
}






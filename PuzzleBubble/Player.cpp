#include <cmath>
#include <iostream>
#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include "dependencies\freeglut\include\GL\glut.h"
#include "Player.h"
#include "Game.h"

#define MAX_ARROW_ROT 1.2f
#define PLAYER_CENTER_CORRECTION_X 1
#define PLAYER_CENTER_CORRECTION_Y 4

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 position)
{
	spritesheet.loadFromFile("images/arrow.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64.f,64.f), glm::vec2(PLAYER_CENTER_CORRECTION_X, PLAYER_CENTER_CORRECTION_Y), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setPosition(position);
	this->position = position;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (arrowAngle < MAX_ARROW_ROT) arrowAngle += 0.05f;
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (arrowAngle > -MAX_ARROW_ROT) arrowAngle -= 0.05f;
	}
	sprite->rotate(arrowAngle);
	sprite->setPosition(position);
}

void Player::render()
{
	sprite->render();
}






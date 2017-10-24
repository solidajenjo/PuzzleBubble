#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include "dependencies\freeglut\include\GL\glut.h"
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();
}

bool Game::update(int deltaTime)
{
	switch (status) {
	case 0:
		menu.update(deltaTime);
		break;
	case 1:
		scene.update(deltaTime);
		break;
	}	
	
	return bPlay;
}

void Game::render(int deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (status) {
	case 0:
		menu.render(deltaTime);
		break;
	case 1:
		scene.render(deltaTime);
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::loadGame()
{
	scene.init();
}

void Game::setStatus(int status)
{
	this->status = status;
	if (status == 0) menu.init();
}






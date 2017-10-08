#include <iostream>
#include <cmath>
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Scene.h"
#include "Game.h"
#include <time.h>


#define SCREEN_X 32
#define SCREEN_Y 16
#define SCREEN_PIX_X 640
#define SCREEN_PIX_Y 480
#define PLAYER_POS_X 320
#define PLAYER_POS_Y 421
#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25
#define BALLS_NUMBER 8
#define BALL_SCALE_X 1.9f
#define BALL_SCALE_Y 1.9f
#define INITIAL_BALL_X 304
#define INITIAL_BALL_Y 410
#define NEXT_BALL_X 214
#define NEXT_BALL_Y 430



Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	srand(time(NULL));
	initShaders();
	ballsTex.loadFromFile("images/balls.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ballsTex.setWrapS(GL_CLAMP_TO_EDGE);
	ballsTex.setWrapT(GL_CLAMP_TO_EDGE);
	ballsTex.setMinFilter(GL_NEAREST);
	ballsTex.setMagFilter(GL_NEAREST);
	
	ballsCoords.push_back(glm::vec2(0.f, 0.f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.25f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.f));
	ballsCoords.push_back(glm::vec2(1.f, 0.25f));
	ballsCoords.push_back(glm::vec2(0.f, 0.25f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.49f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.25f));
	ballsCoords.push_back(glm::vec2(1.0f, .49f));

	ballsCoords.push_back(glm::vec2(0.f, 0.5f)); 
	ballsCoords.push_back(glm::vec2(0.5f, 0.75f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.5f));
	ballsCoords.push_back(glm::vec2(1.f, 0.75f));
	ballsCoords.push_back(glm::vec2(0.f, 0.75f));
	ballsCoords.push_back(glm::vec2(0.5f, 1.f));
	ballsCoords.push_back(glm::vec2(0.5f, 0.75f));
	ballsCoords.push_back(glm::vec2(1.f, 1.f));

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(16.f, 16.f) };
	glm::vec2 texCoords[2];

	int color = rand() % 8;
	texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
	currentBall = Ball::createBall(geom, texCoords, texProgram);
	currentBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));

	color = rand() % 8;
	texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
	nextBall = Ball::createBall(geom, texCoords, texProgram);
	nextBall->setPosition(glm::vec2(NEXT_BALL_X, NEXT_BALL_Y));

	skinTex.loadFromFile("images/skin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	skinTex.setWrapS(GL_CLAMP_TO_EDGE);
	skinTex.setWrapT(GL_CLAMP_TO_EDGE);
	skinTex.setMinFilter(GL_NEAREST);
	skinTex.setMagFilter(GL_NEAREST);
	skin = Sprite::createSprite(glm::ivec2(640.f, 480.f), glm::vec2(0.f, 0.f), glm::vec2(1, 1), &skinTex, &texProgram);
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(PLAYER_POS_X, PLAYER_POS_Y));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime, currentBall);
	skin->setPosition(glm::vec2(SCREEN_PIX_X / 2.f, SCREEN_PIX_Y / 2.f));
	if (movingBall != NULL) {
		movingBall->update(deltaTime);
	}
	if (player->isBallShot()) {
		player->ballShotAcquired();
		glm::vec2 texCoords[2];
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(16.f, 16.f) };
		texCoords[0] = ballsCoords[currentBall->getColor() * 2]; texCoords[1] = ballsCoords[(currentBall->getColor() * 2) + 1];
		movingBall = Ball::createBall(geom, texCoords, texProgram);;
		movingBall->setColor(currentBall->getColor());
		movingBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));
		movingBall->setDirection(player->getArrowDirection());
		currentBall = nextBall;
		currentBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));			
		int color = rand() % 8;
		texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];		
		nextBall = Ball::createBall(geom, texCoords, texProgram);
		nextBall->setColor(color);
		nextBall->setPosition(glm::vec2(NEXT_BALL_X, NEXT_BALL_Y));
	}
}

void Scene::render(int deltaTime)
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();		
	skin->render();	
	currentBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	nextBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	if (movingBall != NULL) movingBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}




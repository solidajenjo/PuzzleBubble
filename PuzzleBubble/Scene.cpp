#include <iostream>
#include <cmath>
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Scene.h"
#include "Game.h"
#include <time.h>

// player/balls definitions
#define SCREEN_X 32
#define SCREEN_Y 16
#define SCREEN_PIX_X 640
#define SCREEN_PIX_Y 480
#define PLAYER_POS_X 319
#define PLAYER_POS_Y 421
#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25
#define BALLS_NUMBER 8
#define BALL_SCALE_X 1.9f
#define BALL_SCALE_Y 1.9f
#define INITIAL_BALL_X 317
#define INITIAL_BALL_Y 425
#define NEXT_BALL_X 234
#define NEXT_BALL_Y 450
//timer definitons
#define SCREEN_MOVEMENT_SOUND 3000 
#define UPDATE_TIME 20000 //20000
//states definitions
#define PLAYING 1
#define UPDATING_BOARD 2
#define DEAD 3
#define STAGE_CLEAR 4
#define MENU 5
#define NUMBER_OF_LEVELS 3
char* levels[] = {"levels/level01.txt", "levels/level02.txt", "levels/level03.txt"};
int level = 0;
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
	status = PLAYING;
	srand(time(NULL));
	initShaders();
	//init balls
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

	int color = rand() % 5;
	texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
	currentBall = Ball::createBall(geom, texCoords, texProgram);
	currentBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));
	currentBall->setColor(color);

	color = rand() % 5;
	texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
	nextBall = Ball::createBall(geom, texCoords, texProgram);
	nextBall->setPosition(glm::vec2(NEXT_BALL_X, NEXT_BALL_Y));
	nextBall->setColor(color);

	//init board
	skinTex.loadFromFile("images/skin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	skinTex.setWrapS(GL_CLAMP_TO_EDGE);
	skinTex.setWrapT(GL_CLAMP_TO_EDGE);
	skinTex.setMinFilter(GL_NEAREST);
	skinTex.setMagFilter(GL_NEAREST);
	skin = Sprite::createSprite(glm::ivec2(640.f, 480.f), glm::vec2(1, 1), &skinTex, &texProgram);
	skin->setSpriteCenter(glm::vec2(SCREEN_X / 2, SCREEN_Y / 2));

	bgTex.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bgTex.setWrapS(GL_REPEAT);
	bgTex.setWrapT(GL_REPEAT);
	bgTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	bgTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	background = Sprite::createSprite(glm::ivec2(800.f, 800.f), glm::vec2(40.0f, 40.0f), &bgTex, &texProgram);
	background->setSpriteCenter(glm::vec2(200.f, 313.f));

	map = TileMap::createTileMap(levels[level++], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//init text
	if (!text.init("fonts/Retro Computer_DEMO.ttf"))
		cout << "Could not load font!!!" << endl;
	//init sound
	gameLoop = new Sound("sounds/mainLoop.wav");
	screenMovementSound = new Sound("sounds/Quake.wav");
	ballStopingSound = new Sound("sounds/ballStoping.wav");
	stageClear = new Sound("sounds/stageClear.wav");
	//init player
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(PLAYER_POS_X, PLAYER_POS_Y));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	//init game parameters
	currentTime = 0.0f;
	score = 0;
	frameCounter = 0;
	updateScreenTimer = UPDATE_TIME;	
}

void Scene::update(int deltaTime)
{	
	queue<int> ballsToExplode = map->getMustExplode(); // format x / y / color * in logic coords TODO change logic to screen
	if (ballsToExplode.size() > 0) {
		//animacion explosion bolas
		score += (ballsToExplode.size() / 3) * 10;
		map->resetMustExplode();
	}
	skin->setPosition(glm::vec2(16.f, 8.f));
	background->setPosition(glm::vec2(386.f, 340.f));
	if (status == DEAD) {
		return;
	}
	currentTime += deltaTime;
	updateScreenTimer -= deltaTime;
	player->update(deltaTime, currentBall);	
	if (updateScreenTimer < SCREEN_MOVEMENT_SOUND && status == PLAYING) screenMovementSound->play();
	if (updateScreenTimer < 0 && status == PLAYING) {		
		status = UPDATING_BOARD;
	}
	switch (status) {
	case STAGE_CLEAR:
		map->render();
		if (updateScreenTimer < 0) {
			updateScreenTimer = 0;
			if (player->anyKeyPressed() && level < NUMBER_OF_LEVELS) {
				map = TileMap::createTileMap(levels[level++], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				status = PLAYING;
				updateScreenTimer = UPDATE_TIME;
			}
		}
		break;
	case UPDATING_BOARD:
		skin->setPosition(glm::vec2(16.f + 2 - (rand()%4), 8.f));		
		if (map->update(deltaTime)) {			
			status = PLAYING;			
			updateScreenTimer = UPDATE_TIME;
			screenMovementSound->stop();
		}
		break;
	case PLAYING:
		gameLoop->playLoop();
		if (map->checkDeath()) {
			status = DEAD;
		}	
		if (map->getBallInserted()) {
			ballStopingSound->play();
			map->ballInsertedAcquired();
		}
		if (movingBall != NULL) {
			textProgram.use();
			textProgram.use();
			textProgram.setUniformMatrix4f("projection", projection);
			textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

			movingBall->update(deltaTime, map);
			if (movingBall->isDelete()) {
				delete(movingBall);
				movingBall = NULL;
				if (map->checkDeath()) {
					status = DEAD;
				}
			}
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
			int color = rand() % 5;
			texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
			nextBall = Ball::createBall(geom, texCoords, texProgram);
			nextBall->setColor(color);
			nextBall->setPosition(glm::vec2(NEXT_BALL_X, NEXT_BALL_Y));
		}
		if (map->getBallsNumber() == 0) {
			status = STAGE_CLEAR;
			gameLoop->stop();
			stageClear->play();
			updateScreenTimer = UPDATE_TIME / 20;
		}
		break;
	}
}

void Scene::render(int deltaTime)
{
	texProgram.use();
	background->render();
	frameCounter++;	
	glm::mat4 modelview;	
	if (status == PLAYING) {
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if(status == DEAD) {
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	}
	else if (status == STAGE_CLEAR) {
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 0.6f, 0.6f, 0.6f, 1.0f);
	}
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	skin->render();
	player->render();			
	currentBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	nextBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	if (movingBall != NULL) {
		movingBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
		textProgram.use();
		textProgram.setUniformMatrix4f("projection", projection);
		textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);	
	}
	text.render("SCORE = " + to_string(score), glm::vec2(240, 24), 16, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	if (status == DEAD && (frameCounter % 100) < 80) {
		text.render("GAME OVER", glm::vec2(50, 224), 80, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		text.render("Press any key to continue.", glm::vec2(130, 324), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	}	
	if (status == STAGE_CLEAR && (frameCounter % 100) < 80) {
		text.render("STAGE CLEARED", glm::vec2(60, 224), 50, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		text.render("Press any key to continue.", glm::vec2(130, 324), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	}
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

	vShader.initFromFile(VERTEX_SHADER, "shaders/text.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/text.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	textProgram.init();
	textProgram.addShader(vShader);
	textProgram.addShader(fShader);
	textProgram.link();
	if (!textProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << textProgram.log() << endl << endl;
	}
	textProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	
}




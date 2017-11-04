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
#define MID_BALL_X 290
#define MID_BALL_Y 442
#define NEXT_BALL_X 234
#define NEXT_BALL_Y 450
#define BUB2_X 235
#define BUB2_Y 420
#define WIN_X 50
#define WIN_Y 100
#define WIN_DISP 420
//timer definitons
#define UPDATE_TIME 15000 //15000 acceptable
#define WAITING_TIME 5500
//music definitions
#define MUSIC_GAP 24000 // too many calls to sound->play()
#define PRE_MOVEMEMENT_SOUND 2000
//states definitions
#define PLAYING 1
#define UPDATING_BOARD 2
#define DEAD 3
#define STAGE_CLEAR 4
#define MENU 5
#define WIN 6
#define NUMBER_OF_LEVELS 5
char* levels[] = {"levels/level01.txt", "levels/level02.txt", "levels/level03.txt",  "levels/level04.txt",  "levels/level05.txt" };
int level = 0;

enum movesHelper {
	STILL, LOADBALL
};

enum movesWinning {
	WINNING
};

Scene::Scene()
{
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (skin != NULL)
		delete skin;
	if (background != NULL)
		delete background;
	if (currentBall != NULL)
		delete currentBall;
	if (nextBall != NULL)
		delete nextBall;
	if (movingBall != NULL)
		delete movingBall;
	if (gameLoop != NULL)
		delete gameLoop;
	if (screenMovementSound != NULL)
		delete screenMovementSound;
	if (ballStopingSound != NULL)
		delete ballStopingSound;
	if (stageClear != NULL)
		delete stageClear;
	if (scoreSound != NULL)
		delete scoreSound;
	if (gameOverSound != NULL)
		delete gameOverSound;
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
	currentBall = Ball::createBall(geom, texCoords, ballProgram);
	currentBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));
	currentBall->setColor(color);

	color = rand() % 5;
	texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
	nextBall = Ball::createBall(geom, texCoords, ballProgram);
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
	level = 0;
	map = TileMap::createTileMap(levels[level++], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//init text
	if (!text.init("fonts/Retro Computer_DEMO.ttf"))
		cout << "Could not load font!!!" << endl;
	//init sound
	gameLoop = new Sound("sounds/mainLoop.wav");
	screenMovementSound = new Sound("sounds/Quake.wav");
	ballStopingSound = new Sound("sounds/ballStoping.wav");
	stageClear = new Sound("sounds/stageClear.wav");
	scoreSound = new Sound("sounds/score.wav");
	gameOverSound = new Sound("sounds/gameOver.wav");
	//init player
	player = new Player();
	player->init(map, texProgram, glm::vec2(PLAYER_POS_X, PLAYER_POS_Y));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	//init game parameters
	currentTime = 0.0f;
	score = 0;
	frameCounter = 0;
	updateScreenTimer = UPDATE_TIME;	
	musicTimer = 0;
	specialBallDogWatch = 5;
	exploding = 0;
	glowing = NULL;
	initAnims();
	framesBub2Anim = 0;	
}

void Scene::update(int deltaTime)
{			
	if (!waitTimer.isFinished()) waitTimer.update(deltaTime);
	skin->setPosition(glm::vec2(16.f, 8.f));
	background->setPosition(glm::vec2(386.f, 340.f));
	if (status == DEAD) {
		screenMovementSound->stop();
		if (waitTimer.isFinished() && player->anyKeyPressed()) Game::instance().setStatus(0);
		return;
	}
	currentTime += deltaTime;
	updateScreenTimer -= deltaTime;
	player->update(deltaTime, currentBall);		
	if (updateScreenTimer - PRE_MOVEMEMENT_SOUND < 0 && status == PLAYING && !screenSoundPlaying) {
		screenMovementSound->play();
		screenSoundPlaying = true;
	}
	if (screenSoundPlaying)	skin->setPosition(glm::vec2(16.f + 3 - (rand() % 6), 8.f));
	if (exploding) {
		int expSize = explosions.size();
		for (int i = 0; i < expSize; ++i) {
			Explosion *exp = explosions.front();
			explosions.pop();
			exp->update(deltaTime);
			explosions.push(exp);
		}
	}
	if (squids.size() > 0) {
		int s = squids.size();
		for (int i = 0; i < s; ++i) {
			Squid *sq = squids.front();
			squids.pop();
			if (!sq->isOut()) {
				sq->update(deltaTime);
				squids.push(sq);
			}
		}
	}
	switch (status) {
	case WIN:
		winnerBub->update(deltaTime);
		if (waitTimer.isFinished() && player->anyKeyPressed()) Game::instance().setStatus(0);
		break;
	case STAGE_CLEAR:
		delete(movingBall);
		movingBall = NULL;
		screenMovementSound->stop();
		screenSoundPlaying = false;
		if (updateScreenTimer < 0) {
			updateScreenTimer = 0;
			if (waitTimer.isFinished() && player->anyKeyPressed() && level < NUMBER_OF_LEVELS) {
				map = TileMap::createTileMap(levels[level++], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				status = PLAYING;
				updateScreenTimer = UPDATE_TIME;
				musicTimer = 0;
			}
			else if (level == 5) {
				status = WIN;
				waitTimer = WaitTimer(WAITING_TIME);
			}
		}
		break;
	case UPDATING_BOARD:		
		if (glowing == NULL && map->update(deltaTime)) {			
			status = PLAYING;			
			updateScreenTimer = UPDATE_TIME;
			screenMovementSound->stop();
			screenSoundPlaying = false;
			map->setIsUpdating(false);
		}
		break;
	case PLAYING:		
		if (framesBub2Anim > 0) {
			framesBub2Anim--;
			if (framesBub2Anim == 10) {
				nextBall->setPosition(glm::vec2(MID_BALL_X, MID_BALL_Y));
			}
		}
		else if (bub2->getAnimation() != STILL) {
			currentBall = nextBall;
			currentBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));
			int color = rand() % 5;
			int specialBall = rand() % 100;
			specialBallDogWatch--;
			if (specialBall < 20 && specialBallDogWatch <= 0) { //chance of special ball 20%
				color = 7;
				specialBallDogWatch = 5; //Minimum 5 balls between each special ball
			}
			glm::vec2 texCoords[2];
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(16.f, 16.f) };
			texCoords[0] = ballsCoords[color * 2]; texCoords[1] = ballsCoords[(color * 2) + 1];
			nextBall = Ball::createBall(geom, texCoords, ballProgram);
			nextBall->setColor(color);
			nextBall->setPosition(glm::vec2(NEXT_BALL_X, NEXT_BALL_Y));
			bub2->changeAnimation(STILL);
		}
		bub2->update(deltaTime);		
		musicTimer -= deltaTime;
		if (map->checkDeath()) {
			status = DEAD;
			delete(movingBall);
			movingBall = NULL;
			gameLoop->stop();
			gameOverSound->play();
			waitTimer = WaitTimer(WAITING_TIME);
			return;
		}
		if (musicTimer <= 0) {
			gameLoop->play();
			musicTimer = MUSIC_GAP;
		}				

		if (glowing != NULL) {
			glowing->update(deltaTime);			
		}

		if (movingBall != NULL) {
			movingBall->update(deltaTime, map);
			if (movingBall->isDelete()) {
				delete(movingBall);
				movingBall = NULL;
				//post ball insertion effects // avoid overlap effects and screen movement
				if (map->getBallInserted()) {
					ballStopingSound->play();
					map->ballInsertedAcquired();
					if (map->getColorInserted() != 7) {
						glowing = new Glow();
						glowing->init(texProgram, map->getPosInserted(), map->getColorInserted());
					}
				}
			}
			if (map->howManyExplosions() > 0) {
				stillExploding = map->howManyExplosions();
				queue<int> ballsToExplode = map->getMustExplode(); // format of queue [color -> y -> x] (x first) in screen coords
				scoreSound->stop();
				score += (ballsToExplode.size() / 3) * 10;
				while (!ballsToExplode.empty()) {
					int x = ballsToExplode.front();
					ballsToExplode.pop();
					int y = ballsToExplode.front();
					ballsToExplode.pop();
					int color = ballsToExplode.front();
					ballsToExplode.pop();
					Explosion *exp = new Explosion();
					exp->init(texProgram, glm::vec2(x, y), color);
					explosions.push(exp);

					Squid *sq = new Squid();
					sq->init(squidProgram, glm::vec2(x, y));
					squids.push(sq);
				}
				exploding = 1;
				map->resetMustExplode();
				scoreSound->play();
			}
		}		
		if (player->isBallShot()) {
			bub2->changeAnimation(LOADBALL);
			framesBub2Anim = 20;
			player->ballShotAcquired();
			glm::vec2 texCoords[2];
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(16.f, 16.f) };
			texCoords[0] = ballsCoords[currentBall->getColor() * 2]; texCoords[1] = ballsCoords[(currentBall->getColor() * 2) + 1];
			movingBall = Ball::createBall(geom, texCoords, ballProgram);;
			movingBall->setColor(currentBall->getColor());
			movingBall->setPosition(glm::vec2(INITIAL_BALL_X, INITIAL_BALL_Y));
			movingBall->setDirection(player->getArrowDirection());	
			currentBall = NULL;
		}
		if (map->getBallsNumber() == 0) {
			status = STAGE_CLEAR;
			waitTimer = WaitTimer(WAITING_TIME);
			gameLoop->stop();
			stageClear->play();
			updateScreenTimer = UPDATE_TIME / 20;
		}
		if (updateScreenTimer < 0 && status == PLAYING && explosions.empty() &&
			glowing == NULL && (movingBall == NULL || !movingBall->isMoving())) {
			status = UPDATING_BOARD;
			map->setIsUpdating(true);
		}
		break;
	}	
}

void Scene::render(int deltaTime)
{
	texProgram.use();	
	frameCounter++;		
	texProgram.setUniformMatrix4f("projection", projection);
	if (status == PLAYING) {		
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if(status == DEAD) {		
		texProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	}
	else if (status == STAGE_CLEAR) {		
		texProgram.setUniform4f("color", 0.6f, 0.6f, 0.6f, 1.0f);
	}
	else if (status == WIN) {
		texProgram.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);
	}
	background->render();
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	skin->render();
	player->render();	
	
	bub2->render();
	if (status == WIN) {
		texProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.0f);
		winnerBub->setPosition(glm::vec2(WIN_X, WIN_Y));
		winnerBub->render();
		winnerBub->setPosition(glm::vec2(WIN_X + WIN_DISP, WIN_Y));
		winnerBub->render();
	}
	ballProgram.use();
	ballProgram.setUniformMatrix4f("projection", projection);
	ballProgram.setUniformMatrix4f("modelview", modelview);
	ballProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (status == PLAYING) {
		ballProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (status == DEAD) {
		ballProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	}
	else if (status == STAGE_CLEAR) {
		ballProgram.setUniform4f("color", 0.6f, 0.6f, 0.6f, 1.0f);
	}
	else if (status == WIN) {
		ballProgram.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);
	}
	if (currentBall != NULL) currentBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	nextBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
	if (exploding) {
		int expSize = explosions.size();
		for (int i = 0; i < expSize; ++i) {
			Explosion *exp = explosions.front();
			explosions.pop();
			if (exp->getState() < 20) {
				explosions.push(exp);
				exp->render();
			}
			else {
				delete exp;
			}					
		}
		if (explosions.size() == 0) {
			exploding = 0;
		}

	}
		
	
	if (movingBall != NULL) {
		movingBall->render(glm::vec2(BALL_SCALE_X, BALL_SCALE_Y), ballsTex);
		textProgram.use();
		textProgram.setUniformMatrix4f("projection", projection);
		textProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);	
	}
	if (glowing != NULL) {
		glowing->render();
		if (glowing->getState() == 10) {
			delete glowing;
			glowing = NULL;
		}
	}
	text.render("LEVEL "+to_string(level)+"             SCORE = " + to_string(score), glm::vec2(210, 24), 12, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	if (status == DEAD && (frameCounter % 100) < 80) {
		text.render("GAME OVER", glm::vec2(50, 224), 80, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		if (waitTimer.isFinished())
			text.render("Press any key to continue.", glm::vec2(130, 324), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	}	
	if (level != 5 && status == STAGE_CLEAR && (frameCounter % 100) < 80) {
		text.render("STAGE CLEARED", glm::vec2(60, 224), 50, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		if (waitTimer.isFinished())
			text.render("Press any key to continue.", glm::vec2(130, 324), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	}	
	if (status == WIN && (frameCounter % 100) < 80) {
		text.render("CONGRATULATIONS", glm::vec2(190, 180), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		text.render("YOU WIN!!!", glm::vec2(246, 214), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
		if (waitTimer.isFinished())
			text.render("Press any key to continue.", glm::vec2(130, 324), 20, glm::vec4(0.02f, 0.96f, 0.15f, 1.f));
	}
	squidProgram.use();
	squidProgram.setUniformMatrix4f("projection", projection);
	if (status == DEAD) squidProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	else squidProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	if (squids.size() > 0) {
		int s = squids.size();
		for (int i = 0; i < s; ++i) {
			Squid *sq = squids.front();
			squids.pop();
			sq->render();
			squids.push(sq);
		}
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


	ballProgram.init();
	ballProgram.addShader(vShader);
	ballProgram.addShader(fShader);
	ballProgram.link();
	if (!ballProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << ballProgram.log() << endl << endl;
	}
	ballProgram.bindFragmentOutput("outColor");

	squidProgram.init();
	squidProgram.addShader(vShader);
	squidProgram.addShader(fShader);
	squidProgram.link();
	if (!squidProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << squidProgram.log() << endl << endl;
	}
	squidProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();

	
}

void Scene::initAnims() {
	bub2Tex.loadFromFile("images/bub2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bub2Tex.setWrapS(GL_CLAMP_TO_EDGE);
	bub2Tex.setWrapT(GL_CLAMP_TO_EDGE);
	bub2Tex.setMinFilter(GL_NEAREST);
	bub2Tex.setMagFilter(GL_NEAREST);
	bub2 = Sprite::createSprite(glm::vec2(70.f, 54.f), glm::vec2(0.125f, 1.f), &bub2Tex, &texProgram);
	bub2->setNumberAnimations(2);
	bub2->setAnimationSpeed(STILL, 2);
	bub2->addKeyframe(STILL, glm::vec2(0.0f, 0.f));
	bub2->addKeyframe(STILL, glm::vec2(0.298f, 0.f));
	bub2->setAnimationSpeed(LOADBALL, 5);
	bub2->addKeyframe(LOADBALL, glm::vec2(0.584f, 0.f));
	bub2->addKeyframe(LOADBALL, glm::vec2(0.71f, 0.f));
	bub2->addKeyframe(LOADBALL, glm::vec2(0.852f, 0.f));	
	bub2->changeAnimation(STILL);
	bub2->setPosition(glm::vec2(BUB2_X, BUB2_Y));

	/////////////////////////////////////////////

	winTex.loadFromFile("images/winning.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winTex.setWrapS(GL_CLAMP_TO_EDGE);
	winTex.setWrapT(GL_CLAMP_TO_EDGE);
	winTex.setMinFilter(GL_NEAREST);
	winTex.setMagFilter(GL_NEAREST);
	winnerBub = Sprite::createSprite(glm::vec2(120.f, 184.f), glm::vec2(1.f / 18.f, 1.f), &winTex, &texProgram);
	winnerBub->setNumberAnimations(1);
	winnerBub->setAnimationSpeed(WINNING, 10);
	winnerBub->addKeyframe(WINNING, glm::vec2(0.0f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.056f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.111f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.166f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.222f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.277f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.332f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.387f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(0.442f, 0.f));
	float disp = 0.497f;
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.0f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.056f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.111f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.166f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.222f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.277f, 0.1f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.332f, 0.1f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.387f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.442f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.387f, 0.f));
	winnerBub->addKeyframe(WINNING, glm::vec2(disp + 0.332f, 0.1f));
	winnerBub->changeAnimation(WINNING);
	winnerBub->setPosition(glm::vec2(WIN_X, WIN_Y));
}




#include <iostream>
#include <cmath>
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Menu.h"
#include "Game.h"

#define SCREEN_X 640
#define SCREEN_Y 480
#define MENU_X 320.f
#define MENU_Y 240.f
#define MENU_TEXT_X 300.f
#define MENU_TEXT_Y 250.f
#define ROTSPEED 0.00002f
#define MAX_ROT 0.05f
#define SELECTOR_X 35.f
#define TIME_BETWEEN_KEYS 100
#define MUSIC_GAP 30000 // time between calls to sound->play() 

glm::vec2 menuCoords[3] = { glm::vec2(SELECTOR_X,90.f), glm::vec2(SELECTOR_X,130.f), glm::vec2(SELECTOR_X,180.f) };

Menu::Menu()
{

}

Menu::~Menu()
{

}


void Menu::init()
{
	initShaders();
	
	menuBackgroundTex.loadFromFile("images/menuBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuBackgroundTex.setWrapS(GL_CLAMP_TO_EDGE);
	menuBackgroundTex.setWrapT(GL_CLAMP_TO_EDGE);
	menuBackgroundTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuBackgroundTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuBackground = Sprite::createSprite(glm::ivec2(704.f, 528.f), glm::vec2(1, 1), &menuBackgroundTex, &texProgram);
	menuBackground->setSpriteCenter(glm::vec2(352.f, 264.f));
	menuBackground->setPosition(glm::vec2(MENU_X, MENU_Y));

	menuTextTex.loadFromFile("images/menuForeground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuTextTex.setWrapS(GL_CLAMP_TO_EDGE);
	menuTextTex.setWrapT(GL_CLAMP_TO_EDGE);
	menuTextTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuTextTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuText = Sprite::createSprite(glm::ivec2(640.f, 512.f), glm::vec2(1, 1), &menuTextTex, &texProgram);
	menuText->setSpriteCenter(glm::vec2(320.f, 256.f));
	menuText->setPosition(glm::vec2(MENU_TEXT_X, MENU_TEXT_Y));

	menuSelectorTex.loadFromFile("images/menuSelector.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuSelectorTex.setWrapS(GL_CLAMP_TO_EDGE);
	menuSelectorTex.setWrapT(GL_CLAMP_TO_EDGE);
	menuSelectorTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuSelectorTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	menuSelector = Sprite::createSprite(glm::ivec2(60.f, 64.5f), glm::vec2(1, 1), &menuSelectorTex, &texProgram);
	menuSelector->setSpriteCenter(glm::vec2(30.f, 32.25f));

	menuLoop = new Sound("sounds/menuLoop.wav");
	musicTimer = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Menu::update(int deltaTime)
{	
	musicTimer -= deltaTime;
	if (musicTimer <= 0) {
		musicTimer = MUSIC_GAP;
		menuLoop->play();
	}
	keyTimer -= deltaTime;
	if (rotAngle < -MAX_ROT || rotAngle > MAX_ROT) rotDirection = -rotDirection;
	rotAngle += ROTSPEED * deltaTime * rotDirection;
	menuBackground->rotate(rotAngle);
	menuSelector->setPosition(menuCoords[menuPos]);
	//menuLoop->playLoop();
	if (Game::instance().getKey(13) && menuPos == 0) {
		menuLoop->stop();		
		delete menuSelector;
		delete menuBackground;
		delete menuText;		
		Game::instance().setStatus(1);
	}
	if (Game::instance().getKey(13) && menuPos == 1) {
		menuLoop->stop();
		delete menuSelector;
		delete menuBackground;
		delete menuText;
		Game::instance().loadInstructions(deltaTime);
		Game::instance().setStatus(2);
	}
	if (Game::instance().getKey(13) && menuPos == 2) {
		menuLoop->stop();
		delete menuSelector;
		delete menuBackground;
		delete menuText;
		Game::instance().loadCredits(deltaTime);
		Game::instance().setStatus(3);
	}
	if (Game::instance().getSpecialKey(101) && menuPos > 0 && keyTimer <= 0) {
		menuPos--;
		keyTimer = TIME_BETWEEN_KEYS;
	}
	if (Game::instance().getSpecialKey(103) && menuPos < 2 && keyTimer <= 0) {
		menuPos++;
		keyTimer = TIME_BETWEEN_KEYS;
	}
	if (keyTimer <= 0) keyTimer = 0;
}

void Menu::render(int deltaTime)
{
	texProgram.use();
	glm::mat4 modelview;
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	menuBackground->render();
	menuText->render();
	menuSelector->render();
}

void Menu::initShaders()
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




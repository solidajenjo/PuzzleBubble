#include "Credits.h"
#include <iostream>
#include <cmath>
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Game.h"

#define SCREEN_X 640
#define SCREEN_Y 480
#define MENU_X 320.f
#define MENU_Y 240.f
#define CRED_TEXT_X 320.f
#define CRED_TEXT_Y 240.f
#define ROTSPEED 0.00002f
#define MAX_ROT 0.05f
#define SELECTOR_X 35.f
#define TIME_BETWEEN_KEYS 100
#define MUSIC_GAP 30000 // time between calls to sound->play() 

Credits::Credits()
{
}


Credits::~Credits()
{
}

void Credits::init(int deltaTime)
{
	initShaders();

	credBackgroundTex.loadFromFile("images/menuBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credBackgroundTex.setWrapS(GL_CLAMP_TO_EDGE);
	credBackgroundTex.setWrapT(GL_CLAMP_TO_EDGE);
	credBackgroundTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	credBackgroundTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	credBackground = Sprite::createSprite(glm::ivec2(704.f, 528.f), glm::vec2(1, 1), &credBackgroundTex, &texProgram);
	credBackground->setSpriteCenter(glm::vec2(352.f, 264.f));
	credBackground->setPosition(glm::vec2(MENU_X, MENU_Y));
	rotAngle += ROTSPEED * deltaTime * rotDirection;
	credBackground->rotate(rotAngle);

	creditsTex.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsTex.setWrapS(GL_CLAMP_TO_EDGE);
	creditsTex.setWrapT(GL_CLAMP_TO_EDGE);
	creditsTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	creditsTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	credits = Sprite::createSprite(glm::ivec2(640.f, 480.f), glm::vec2(1, 1), &creditsTex, &credTexProg);
	credits->setSpriteCenter(glm::vec2(320.f, 240.f));
	credits->setPosition(glm::vec2(CRED_TEXT_X, CRED_TEXT_Y));

	credLoop = new Sound("sounds/menuLoop.wav");
	musicTimer = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Credits::update(int deltaTime)
{
	musicTimer -= deltaTime;
	if (musicTimer <= 0) {
		musicTimer = MUSIC_GAP;
		credLoop->play();
	}
	if (rotAngle < -MAX_ROT || rotAngle > MAX_ROT) rotDirection = -rotDirection;
	rotAngle += ROTSPEED * deltaTime * rotDirection;
	credBackground->rotate(rotAngle);
	//menuLoop->playLoop();
	if (Game::instance().getKey(32)) {
		credLoop->stop();
		delete credBackground;
		delete credits;
		delete credLoop;
		Game::instance().setStatus(0);
	}
}

void Credits::render(int deltaTime)
{
	texProgram.use();
	glm::mat4 modelview;
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	credBackground->render();
	credTexProg.use();
	credTexProg.setUniformMatrix4f("projection", projection);
	credTexProg.setUniform4f("color", 1.f, 1.f, 1.f, 1.0f);
	credTexProg.setUniformMatrix4f("modelview", modelview);
	credTexProg.setUniform2f("texCoordDispl", 0.f, 0.f);
	credits->render();
}

void Credits::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");

	credTexProg.init();
	credTexProg.addShader(vShader);
	credTexProg.addShader(fShader);
	credTexProg.link();
	if (!credTexProg.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << credTexProg.log() << endl << endl;
	}
	credTexProg.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
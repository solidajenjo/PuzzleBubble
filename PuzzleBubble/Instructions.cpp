#include "Instructions.h"
#include <iostream>
#include <cmath>
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Game.h"

#define SCREEN_X 640
#define SCREEN_Y 480
#define MENU_X 320.f
#define MENU_Y 240.f
#define INST_TEXT_X 320.f
#define INST_TEXT_Y 240.f
#define ROTSPEED 0.00002f
#define MAX_ROT 0.05f
#define SELECTOR_X 35.f
#define TIME_BETWEEN_KEYS 100
#define MUSIC_GAP 30000 // time between calls to sound->play() 

Instructions::Instructions()
{
}


Instructions::~Instructions()
{
}

void Instructions::init(int deltaTime)
{
	initShaders();

	instBackgroundTex.loadFromFile("images/menuBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instBackgroundTex.setWrapS(GL_CLAMP_TO_EDGE);
	instBackgroundTex.setWrapT(GL_CLAMP_TO_EDGE);
	instBackgroundTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	instBackgroundTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	instBackground = Sprite::createSprite(glm::ivec2(704.f, 528.f), glm::vec2(1, 1), &instBackgroundTex, &texProgram);
	instBackground->setSpriteCenter(glm::vec2(352.f, 264.f));
	instBackground->setPosition(glm::vec2(MENU_X, MENU_Y));
	rotAngle += ROTSPEED * deltaTime * rotDirection;
	instBackground->rotate(rotAngle);

	instructionsTex.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructionsTex.setWrapS(GL_CLAMP_TO_EDGE);
	instructionsTex.setWrapT(GL_CLAMP_TO_EDGE);
	instructionsTex.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	instructionsTex.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	instructions = Sprite::createSprite(glm::ivec2(640.f, 480.f), glm::vec2(1, 1), &instructionsTex, &instTexProg);
	instructions->setSpriteCenter(glm::vec2(320.f, 240.f));
	instructions->setPosition(glm::vec2(INST_TEXT_X, INST_TEXT_Y));

	instLoop = new Sound("sounds/menuLoop.wav");
	musicTimer = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Instructions::update(int deltaTime)
{
	musicTimer -= deltaTime;
	if (musicTimer <= 0) {
		musicTimer = MUSIC_GAP;
		instLoop->play();
	}
	if (rotAngle < -MAX_ROT || rotAngle > MAX_ROT) rotDirection = -rotDirection;
	rotAngle += ROTSPEED * deltaTime * rotDirection;
	instBackground->rotate(rotAngle);
	//menuLoop->playLoop();
	if (Game::instance().getKey(32)) {
		instLoop->stop();
		delete instBackground;
		delete instructions;
		//Game::instance().loadGame();
		Game::instance().setStatus(0);
	}
}

void Instructions::render(int deltaTime)
{
	texProgram.use();
	glm::mat4 modelview;
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 0.2f, 0.2f, 0.2f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	instBackground->render();
	instTexProg.use();
	instTexProg.setUniformMatrix4f("projection", projection);
	instTexProg.setUniform4f("color", 1.f, 1.f, 1.f, 1.0f);
	instTexProg.setUniformMatrix4f("modelview", modelview);
	instTexProg.setUniform2f("texCoordDispl", 0.f, 0.f);
	instructions->render();
}

void Instructions::initShaders()
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

	instTexProg.init();
	instTexProg.addShader(vShader);
	instTexProg.addShader(fShader);
	instTexProg.link();
	if (!instTexProg.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << instTexProg.log() << endl << endl;
	}
	instTexProg.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
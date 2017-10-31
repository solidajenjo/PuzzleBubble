#pragma once

#include "dependencies\glm\glm\glm.hpp"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Sound.h"
class Instructions
{
public:
	Instructions();
	~Instructions();
	void init(int deltaTime);
	void update(int deltaTime);
	void render(int deltaTime);
private:
	glm::mat4 projection;
	Sprite *instBackground, *instructions;
	Texture instBackgroundTex, instructionsTex;
	Sound *instLoop;
	ShaderProgram texProgram, instTexProg;
	void initShaders();
	float rotDirection = 1.f;
	float rotAngle = 0.f;
	int musicTimer = 0;
};


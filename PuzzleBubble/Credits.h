#pragma once

#include "dependencies\glm\glm\glm.hpp"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Sound.h"
class Credits
{
public:
	Credits();
	~Credits();
	void init(int deltaTime);
	void update(int deltaTime);
	void render(int deltaTime);
private:
	glm::mat4 projection;
	Sprite *credBackground, *credits;
	Texture credBackgroundTex, creditsTex;
	Sound *credLoop;
	ShaderProgram texProgram, credTexProg;
	void initShaders();
	float rotDirection = 1.f;
	float rotAngle = 0.f;
	int musicTimer = 0;
};


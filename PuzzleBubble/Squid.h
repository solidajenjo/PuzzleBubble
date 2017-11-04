#pragma once
#include "Sprite.h"
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"

class Squid
{
public:
	Squid();
	~Squid();
	void init(ShaderProgram &shaderProgram, glm::vec2 &position);
	void update(int deltaTime);
	void render();
	bool isOut();
private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 direction;
	glm::vec2 position;
	Texture spritesheet;	
	ShaderProgram *shaderProgram;	
	int wichSquid, time;
	vector<glm::vec2> squidCoords;
};


#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Texture.h"
#include "ShaderProgram.h"


class Ball
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Ball *createBall(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	Ball(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	void render(glm::vec2 scale, Texture &tex) const;
	void update(int deltaTime);
	void setPosition(glm::vec2 position);
	void setDirection(glm::vec2 direction);
	void setColor(int color);
	int getColor();
	void free();
	bool isMoving();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	ShaderProgram *program;
	glm::vec2 direction;
	glm::vec2 position;
	bool moving = false;
	int color;
};


#endif // _TEXTURED_QUAD_INCLUDE


#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include <GL/gl.h>
#include "Ball.h"

#define SPEED 0.7f
#define LEFT_MARGIN 192
#define RIGHT_MARGIN 417


Ball *Ball::createBall(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	Ball *quad = new Ball(geom, texCoords, program);	
	return quad;
}


Ball::Ball(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	this->program = &program;
	float vertices[24] = {geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y, 
												geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y, 
												geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y, 
												geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y, 
												geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y, 
												geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void Ball::render(glm::vec2 scale, Texture &tex) const
{
	glm::mat4 modelview;
	program->use();
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	modelview = glm::scale(modelview, glm::vec3(scale.x, scale.y, 0.f));
	program->setUniformMatrix4f("modelview", modelview);

	glEnable(GL_TEXTURE_2D);
	tex.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Ball::update(int deltaTime)
{
	if (position.x > RIGHT_MARGIN) direction.x = -direction.x;
	if (position.x < LEFT_MARGIN) direction.x = -direction.x;
	position += glm::vec2(direction.x * deltaTime * (SPEED), direction.y * deltaTime * (SPEED));
}

void Ball::setPosition(glm::vec2 position)
{
	this->position = position;
}


void Ball::setDirection(glm::vec2 direction)
{
	this->direction = direction;
	moving = true;
}

void Ball::setColor(int color)
{
	this->color = color;
}

int Ball::getColor()
{
	return color;
}

void Ball::free()
{
	glDeleteBuffers(1, &vbo);
}

bool Ball::isMoving()
{
	return moving;
}


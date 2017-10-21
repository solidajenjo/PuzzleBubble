#include "dependencies\glew-1.13.0\include\GL\glew.h"
#include <GL/gl.h>
#include "Ball.h"

#define SPEED 0.8f
#define LEFT_MARGIN 208
#define RIGHT_MARGIN 426
#define CENTER_CORRECTION_X 13.0f
#define CENTER_CORRECTION_Y 19.0f


Ball *Ball::createBall(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	Ball *quad = new Ball(geom, texCoords, program);	
	return quad;
}


Ball::Ball(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program)
{
	nextFramePosition = vector<glm::vec2>(3, glm::vec2(0.f, 0.f));
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
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x - CENTER_CORRECTION_X, position.y - CENTER_CORRECTION_Y, 0.f));
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

void Ball::update(int deltaTime, TileMap *tileMap)
{			
 	if (position.x > RIGHT_MARGIN) direction.x = -direction.x;
	if (position.x < LEFT_MARGIN) direction.x = -direction.x;	
	float dXSpeed = direction.x * SPEED *deltaTime;
	float dYSpeed = direction.y * SPEED *deltaTime;
	position.x += dXSpeed;
	position.y += dYSpeed;

	/*Collision with ocupied cells with 3 points in fan form like 

			\|/
			 O

	*/
	nextFramePosition[1].x = position.x + direction.x * 28.f;
	nextFramePosition[1].y = position.y + direction.y * 28.f;

	if (position.x > RIGHT_MARGIN) nextFramePosition[1].x = position.x - direction.x * 28;
	if (position.x < LEFT_MARGIN) nextFramePosition[1].x = position.x - direction.x * 28;

	glm::vec2 sideVector = glm::vec2(position.x + direction.x * 20.f, position.y + direction.y * 20.f); //side vectors slightly shorter
	glm::mat4 rotMat = glm::translate(glm::mat4(1.0f), glm::vec3(sideVector, 0.f));
	rotMat = glm::rotate(rotMat, 0.5f, glm::vec3(0.f, 0.f, 1.f));
	rotMat = glm::translate(rotMat, glm::vec3(-position.x, -position.y, 0.f));
	nextFramePosition[2] = glm::vec2(rotMat * glm::vec4(sideVector, 0.f, 1.f));

	rotMat = glm::translate(glm::mat4(1.0f), glm::vec3(sideVector, 0.f));
	rotMat = glm::rotate(rotMat, -0.5f, glm::vec3(0.f, 0.f, 1.f));
	rotMat = glm::translate(rotMat, glm::vec3(-position.x, -position.y, 0.f));
	nextFramePosition[0] = glm::vec2(rotMat * glm::vec4(sideVector, 0.f, 1.f));

	int xDir;
	if (direction.x < 0) xDir = -1;
	else if (direction.x > 0) xDir = 1;
	else xDir = 0;
	int nextCellContent = tileMap->screenToTileCellContent(nextFramePosition[0], xDir) +
		tileMap->screenToTileCellContent(nextFramePosition[1], xDir) +
		tileMap->screenToTileCellContent(nextFramePosition[2], xDir);
	if (nextCellContent != 0) {
		tileMap->insertBall(glm::vec2(position.x, position.y - CENTER_CORRECTION_Y * 0.8f), -xDir, color);
		moving = false;
		deleteBall = true;
	}
}

void Ball::setPosition(glm::vec2 position)
{
	this->position = position;
	this->nextFramePosition[0] = position;
}

glm::vec2 Ball::getPosition()
{
	return position;
}

vector<glm::vec2> Ball::getNextFramePos()
{
	return nextFramePosition;
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

bool Ball::isDelete()
{
	return deleteBall;
}


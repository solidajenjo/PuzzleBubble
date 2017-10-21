#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include "dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "Texture.h"
#include "ShaderProgram.h"
#include "TileMap.h"

class Ball
{

public:	
	static Ball *createBall(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	Ball(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program);

	void render(glm::vec2 scale, Texture &tex) const;
	void update(int deltaTime, TileMap *tileMap);
	void setPosition(glm::vec2 position);
	glm::vec2 getPosition();
	vector<glm::vec2> getNextFramePos();
	void setDirection(glm::vec2 direction);
	void setColor(int color);
	int getColor();
	void free();
	bool isMoving();
	bool isDelete();
private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	ShaderProgram *program;
	glm::vec2 direction;
	glm::vec2 position;
	vector<glm::vec2> nextFramePosition;
	bool moving = false;
	bool deleteBall = false;
	int color;
	int nextCellContent = 0;
};


#endif //_BALL_INCLUDE


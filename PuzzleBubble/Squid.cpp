#include "Squid.h"

#define GRAVITY 0.001

Squid::Squid()
{
}


Squid::~Squid()
{
}

void Squid::init(ShaderProgram & shaderProgram, glm::vec2 & position)
{
	this->shaderProgram = &shaderProgram;
	spritesheet.loadFromFile("images/squids.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_REPEAT);
	spritesheet.setWrapT(GL_REPEAT);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	squidCoords.push_back(glm::vec2(0.f, 0.f));
	squidCoords.push_back(glm::vec2(0.5f, 0.22f));
	squidCoords.push_back(glm::vec2(0.f, 0.22f));
	squidCoords.push_back(glm::vec2(0.5f, 0.48f));
	squidCoords.push_back(glm::vec2(0.f, 0.48f));
	squidCoords.push_back(glm::vec2(0.5f, 0.77f));
	squidCoords.push_back(glm::vec2(0.f, 0.77f));
	squidCoords.push_back(glm::vec2(0.5f, 1.f));

	squidCoords.push_back(glm::vec2(0.5f, 0.f));
	squidCoords.push_back(glm::vec2(1.f, 0.22f));
	squidCoords.push_back(glm::vec2(0.5f, 0.22f));
	squidCoords.push_back(glm::vec2(1.f, 0.48f));
	squidCoords.push_back(glm::vec2(0.5f, 0.48f));
	squidCoords.push_back(glm::vec2(1.f, 0.77f));
	squidCoords.push_back(glm::vec2(0.5f, 0.77f));
	squidCoords.push_back(glm::vec2(1.f, 1.f));


	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(32.f, 50.f) };
	glm::vec2 texCoords[2];

	int squid = rand() % 8;
	texCoords[0] = squidCoords[squid * 2]; texCoords[1] = squidCoords[(squid * 2) + 1];
	
	direction.x = 3 - (rand() % 6);
	direction.y = -4.f;
	this->position = position;
	float vertices[24] = { geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = shaderProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	time = 0;
}

void Squid::update(int deltaTime)
{
	time++;
	position = position + direction;
	direction.y += deltaTime * GRAVITY * time;
}

void Squid::render()
{
	glm::mat4 modelview;
	shaderProgram->use();
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));	
	shaderProgram->setUniformMatrix4f("modelview", modelview);

	glEnable(GL_TEXTURE_2D);
	spritesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

bool Squid::isOut()
{
	return position.y > 480.0f;
}

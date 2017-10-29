#include "Explosion.h"

enum explosionAnims {
	BLUE, GREEN, RED, ORANGE, YELLOW
};

Explosion::Explosion()
{
}


Explosion::~Explosion()
{
}

void Explosion::init(ShaderProgram &shaderProgram, glm::vec2 &position, int color) {
	state = 0;
	spritesheet.loadFromFile("images/explosions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_REPEAT);
	spritesheet.setWrapT(GL_REPEAT);
	spritesheet.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	spritesheet.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	sprite = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(BLUE, 10);
	sprite->addKeyframe(BLUE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.8f, 0.f));
	sprite->setAnimationSpeed(GREEN, 10);
	sprite->addKeyframe(GREEN, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.8f, 0.2f));
	sprite->setAnimationSpeed(RED, 10);
	sprite->addKeyframe(RED, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.8f, 0.4f));
	sprite->setAnimationSpeed(ORANGE, 10);
	sprite->addKeyframe(ORANGE, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.8f, 0.6f));
	sprite->setAnimationSpeed(YELLOW, 10);
	sprite->addKeyframe(YELLOW, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.2f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.4f, 0.8F));
	sprite->addKeyframe(YELLOW, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.8f, 0.8f));
	sprite->changeAnimation(color - 1);
	sprite->setPosition(position);
}

void Explosion::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Explosion::render() {
	sprite->render();
	++state;
	if (state == 100) {
		sprite->free();
		delete this;
	}
}

int Explosion::getState() {
	return state;
}
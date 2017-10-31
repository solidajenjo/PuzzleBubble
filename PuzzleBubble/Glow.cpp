#include "Glow.h"

enum glowAnims{
	BLUE, GREEN, RED, ORANGE, YELLOW
};

Glow::Glow()
{
}


Glow::~Glow()
{
}

void Glow::init(ShaderProgram &shaderProgram, glm::vec2 &position, int color) {
	state = 0;
	spritesheet.loadFromFile("images/brillos.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_REPEAT);
	spritesheet.setWrapT(GL_REPEAT);
	spritesheet.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	spritesheet.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	sprite = Sprite::createSprite(glm::vec2(30.f, 30.f), glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(BLUE, 30);
	sprite->addKeyframe(BLUE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(BLUE, glm::vec2(0.8f, 0.f));
	sprite->setAnimationSpeed(GREEN, 30);
	sprite->addKeyframe(GREEN, glm::vec2(0.f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(GREEN, glm::vec2(0.8f, 0.2f));
	sprite->setAnimationSpeed(RED, 30);
	sprite->addKeyframe(RED, glm::vec2(0.f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(RED, glm::vec2(0.8f, 0.4f));
	sprite->setAnimationSpeed(ORANGE, 30);
	sprite->addKeyframe(ORANGE, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(ORANGE, glm::vec2(0.8f, 0.6f));
	sprite->setAnimationSpeed(YELLOW, 30);
	sprite->addKeyframe(YELLOW, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.2f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.4f, 0.8F));
	sprite->addKeyframe(YELLOW, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(YELLOW, glm::vec2(0.8f, 0.8f));
	sprite->changeAnimation(color);
	sprite->setPosition(position);
}

void Glow::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Glow::render() {
	sprite->render();
	++state;
	if (state == 10) {
		sprite->free();
	}
}

int Glow::getState() {
	return state;
}
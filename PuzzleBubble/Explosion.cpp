#include "Explosion.h"

enum explosionAnims {
	EXPLODING
};

Explosion::Explosion()
{
}


Explosion::~Explosion()
{
}

void Explosion::init(ShaderProgram &shaderProgram, glm::vec2 &position) {
	state = 0;
	spritesheet.loadFromFile("images/redexplosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_REPEAT);
	spritesheet.setWrapT(GL_REPEAT);
	spritesheet.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	spritesheet.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	sprite = Sprite::createSprite(glm::vec2(32.f, 32.f), glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(EXPLODING, 10);
	sprite->addKeyframe(EXPLODING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.75f, 0.f));
	sprite->changeAnimation(EXPLODING);
	sprite->setPosition(position);
}

void Explosion::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Explosion::render() {
	sprite->render();
	++state;
	if (state == 60) {
		sprite->free();
		delete this;
	}
}

int Explosion::getState() {
	return state;
}
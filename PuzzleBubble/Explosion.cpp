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
	sprite = Sprite::createSprite(glm::vec2(16.f, 16.f), glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(EXPLODING, 15);
	sprite->addKeyframe(EXPLODING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(EXPLODING, glm::vec2(0.75f, 0.f));
	sprite->changeAnimation(0);
	sprite->setPosition(position);
}

void Explosion::update(int deltaTime) {
	sprite->update(deltaTime);
	++state;
	if (state == 18) {
		sprite->free();
		delete this;
	}
}

void Explosion::render() {
	sprite->changeAnimation(EXPLODING);
	sprite->render();
}

int Explosion::getState() {
	return state;
}
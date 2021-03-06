#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Quad.h"
#include "Ball.h"
#include "Text.h"
#include "Sound.h"
#include "Explosion.h"
#include "Glow.h"
#include "WaitTimer.h"
#include "Squid.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render(int deltaTime);

private:
	void initShaders();
	void initAnims();
	TileMap *map;
	Player *player;
	ShaderProgram texProgram, textProgram, ballProgram, squidProgram;
	float currentTime;
	int updateScreenTimer;
	glm::mat4 projection;
	Sprite *skin, *background, *bub2, *winnerBub;
	Texture skinTex, ballsTex, bgTex, explosionTex, bub2Tex, glowTex, winTex;	
	vector<glm::vec2> ballsCoords; //balls tex coords
	queue<Explosion*> explosions; //explosions go here 
	queue<Squid*> squids;
	Ball *currentBall, *nextBall, *movingBall = NULL;
	int status, score, frameCounter, musicTimer, specialBallDogWatch, stillExploding;
	Glow *glowing;
	int framesBub2Anim;
	Text text;
	Sound *gameLoop, *screenMovementSound, *ballStopingSound, *stageClear, *scoreSound, *gameOverSound;
	bool screenSoundPlaying = false;
	int exploding; //0 if no balls are exploding, 1..5 if they are, indicating the sprite
	queue<int> ballsExploding; //x, y and color of the exploding balls while they are still exploding
	queue<Ball> qBallsExploding; //to enqueue explosions and render them
	WaitTimer waitTimer;
};


#endif // _SCENE_INCLUDE


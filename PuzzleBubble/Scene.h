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
	TileMap *map;
	Player *player;
	ShaderProgram texProgram, textProgram;
	float currentTime;
	int updateScreenTimer;
	glm::mat4 projection;
	Sprite *skin, *background;
	Texture skinTex, ballsTex, bgTex, explosionTex;	
	vector<glm::vec2> ballsCoords; //balls tex coords
	vector<glm::vec2> exploCoords; //explosion tex coords
	Ball *currentBall, *nextBall, *movingBall = NULL;
	int status, score, frameCounter, musicTimer, specialBallDogWatch, stillExploding;
	Text text;
	Sound *gameLoop, *screenMovementSound, *ballStopingSound, *stageClear, *scoreSound, *gameOverSound;
	bool screenSoundPlaying = false;
	int exploding = 0; //0 if no balls are exploding, 1..4 if they are, indicating the sprite
	queue<int> ballsExploding; //x, y and color of the exploding balls while they are still exploding
	queue<Ball> qBallsExploding; //to enqueue explosions and render them
};


#endif // _SCENE_INCLUDE


#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include <vector>
#include <queue>
#include "Texture.h"
#include "Sound.h"
#include "Sprite.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	vector<vector<int> > getLogicMatrix();
	int getTileSize() const { return tileSize; }
	int screenToTileCellContent(glm::vec2 screenPos, bool special);
	void insertBall(glm::vec2 position, int color);
	bool update(int deltaTime);
	bool checkDeath();
	queue<int> getMustExplode();
	int howManyExplosions();
	int getBallsNumber();
	void resetMustExplode();
	void ballInsertedAcquired();
	bool getBallInserted();
	void initPress();
	glm::vec2 getPosInserted();
	int getColorInserted();
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	vector<vector<int> > logicMatrix;
	vector<vector<int> > logicToMapMatrix;
	vector<vector<glm::vec2> >logicToScreen;
	vector<int>	mapToLogicMatrix;
	void incLineOffset();
	void checkExplosions(glm::vec2 newBallPos, int color);	
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int lineOffset = 0;
	int ballsNumber = 0;
	float pixelOffset = 0.f;
	glm::vec2 minCoordsRedraw;
	ShaderProgram programRedraw, pressProgram;
	queue<int> mustExplode;
	bool fillLogToMap = true;
	bool ballInserted = false;
	Texture pressTex;
	Sprite *press;
	glm::vec2 posInserted;
	int colorInserted;
};


#endif // _TILE_MAP_INCLUDE



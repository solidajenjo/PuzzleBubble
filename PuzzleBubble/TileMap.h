#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include <vector>
#include "Texture.h"
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
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	vector<vector<int> > logicMatrix;
	void incLineOffset();
private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int lineOffset = 0;
};


#endif // _TILE_MAP_INCLUDE



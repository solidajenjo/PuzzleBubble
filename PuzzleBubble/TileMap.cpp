#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

#define PIXEL_MOVE_PER_FRAME .2f
#define DEATH_LINE 13
#define OFFSET_H 145.f;
#define OFFSET_V 6.f;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	logicMatrix = vector<vector<int> >(mapSize.y + lineOffset, vector<int>(mapSize.x * 2, 0));
	logicToMapMatrix = vector<vector<int> >(mapSize.y + lineOffset, vector<int>(mapSize.x * 2, 0));
	mapToLogicMatrix = vector<int>(mapSize.x * mapSize.y * 2, 0);
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	this->minCoordsRedraw = minCoords;
	this->programRedraw = program;
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	ballsNumber = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			int mapPos = j * mapSize.x + i;
			tile = map[mapPos];
			int xLog;
			if (j % 2 == 1 - (lineOffset % 2)) xLog = i * 2;
			else xLog = i * 2 + 1;
			logicMatrix[j][xLog] = tile; // check color combination matrix
			logicToMapMatrix[j][xLog] = mapPos;
			mapToLogicMatrix[mapPos * 2] = xLog;
			mapToLogicMatrix[mapPos * 2 + 1] = j;
			if(tile != 0 && tile != 9)
			{
				// Non-empty tile
				ballsNumber++;
				nTiles++;
				float offsetH = OFFSET_H;
				float offsetV = OFFSET_V;
				if (j % 2 == 1 - (lineOffset % 2)) offsetH -= 13.f;
				posTile = glm::vec2(offsetH + minCoords.x + i * tileSize, pixelOffset + offsetV + minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y - 1);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y - 1);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y - 1);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}
	fillLogToMap = false;
	if (nTiles > 0) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	}
}

void TileMap::incLineOffset()
{
	lineOffset++;
}

int TileMap::getBallsNumber()
{
	return ballsNumber;
}

int logicXtoScreen(int x) {
	return 0;
}

int logicYtoScreen(int x) {
	return 0;
}

void TileMap::checkExplosions(glm::vec2 newBallPos, int color)
{
	queue<int> Q;
	queue<int> eraseInMap;
	queue<int> hangingBalls;
	mustExplode = queue<int>();
	Q.push(newBallPos.x);
	Q.push(newBallPos.y);
	eraseInMap.push(newBallPos.x);
	eraseInMap.push(newBallPos.y);
	mustExplode.push(newBallPos.x);
	mustExplode.push(newBallPos.y);
	mustExplode.push(color);

	vector <vector<bool> > visited = vector<vector<bool> >(mapSize.y + lineOffset, vector<bool>(mapSize.x * 2, false));
	//check color connection >= 3
	while (!Q.empty()) {
		int x = Q.front(); Q.pop();
		int y = Q.front(); Q.pop();
		if (y > 0 && x > 0 && !visited[y - 1][x - 1] && logicMatrix[y - 1][x - 1] == color) {
			visited[y - 1][x - 1] = true;			
			Q.push(x - 1); Q.push(y - 1);
			eraseInMap.push(x - 1); eraseInMap.push(y - 1);
			mustExplode.push(x - 1); mustExplode.push(y - 1); mustExplode.push(color);
		}
		if (y > 0 && (x + 1) < logicMatrix[0].size() - 1 && !visited[y - 1][x + 1] && logicMatrix[y - 1][x + 1] == color) {
			visited[y - 1][x + 1] = true;			
			Q.push(x + 1); Q.push(y - 1);
			eraseInMap.push(x + 1); eraseInMap.push(y - 1);
			mustExplode.push(x + 1); mustExplode.push(y - 1); mustExplode.push(color);
		}
		if ((y + 1) < logicMatrix.size() && x > 0 && !visited[y + 1][x - 1] && logicMatrix[y + 1][x - 1] == color) {
			visited[y + 1][x - 1] = true;
			Q.push(x - 1); Q.push(y + 1);
			eraseInMap.push(x - 1); eraseInMap.push(y + 1);
			mustExplode.push(x - 1); mustExplode.push(y + 1); mustExplode.push(color);
		}
		if ((y + 1) < logicMatrix.size() && (x + 1) < logicMatrix[0].size() - 1 && !visited[y + 1][x + 1] && logicMatrix[y + 1][x + 1] == color) {
			visited[y + 1][x + 1] = true;
			Q.push(x + 1); Q.push(y + 1);
			eraseInMap.push(x + 1); eraseInMap.push(y + 1);
			mustExplode.push(x + 1); mustExplode.push(y + 1); mustExplode.push(color);
		}
		if ((x + 1) < logicMatrix[0].size() - 1 && !visited[y][x + 2] && logicMatrix[y][x + 2] == color) {
			visited[y][x + 2] = true;
			Q.push(x + 2); Q.push(y);
			eraseInMap.push(x + 2); eraseInMap.push(y);
			mustExplode.push(x + 2); mustExplode.push(y); mustExplode.push(color);
		}
		if (x > 1 && !visited[y][x - 2] && logicMatrix[y][x - 2] == color) {
			visited[y][x - 2] = true;
			Q.push(x - 2); Q.push(y);
			eraseInMap.push(x - 2); eraseInMap.push(y);
			mustExplode.push(x - 2); mustExplode.push(y); mustExplode.push(color);
		}
	}
	//update map
	if (mustExplode.size() >= 9) {
		while (eraseInMap.size() > 0) {
			int x = eraseInMap.front(); eraseInMap.pop();
			int y = eraseInMap.front(); eraseInMap.pop();
			map[logicToMapMatrix[y][x]] = 0;
		}
		this->prepareArrays(minCoordsRedraw, programRedraw);
	}
	else mustExplode = queue<int>();	
	//check "flying" balls
	//if not exists a path to the top of the board the ball is "flying"
	for (int i = 1; i < logicMatrix.size(); ++i) {
		for (int j = 0; j < logicMatrix[i].size(); ++j) {
			if (logicMatrix[i][j] != 0) {
				Q = queue<int>();
				Q.push(j); Q.push(i);
				bool hanging = false;
				if (i > 6) {
					int k = 0;
				}
				visited = vector<vector<bool> >(mapSize.y + lineOffset, vector<bool>(mapSize.x * 2, false));
				visited[i][j] = true;
				while (!Q.empty() && !hanging) {
					int x = Q.front(); Q.pop();
					int y = Q.front(); Q.pop();
					if (logicMatrix[y][x] == 9) hanging = true;
					if (y > 0 && x > 0 && !visited[y - 1][x - 1] && logicMatrix[y - 1][x - 1] != 0) {
						visited[y - 1][x - 1] = true;
						Q.push(x - 1); Q.push(y - 1);
					}
					if (y > 0 && (x + 1) < logicMatrix[0].size() - 1 && !visited[y - 1][x + 1] && logicMatrix[y - 1][x + 1] != 0) {
						visited[y - 1][x + 1] = true;
						Q.push(x + 1); Q.push(y - 1);
					}
					if ((y + 1) < logicMatrix.size() && x > 0 && !visited[y + 1][x - 1] && logicMatrix[y + 1][x - 1] != 0) {
						visited[y + 1][x - 1] = true;
						Q.push(x - 1); Q.push(y + 1);
					}
					if ((y + 1) < logicMatrix.size() && (x + 1) < logicMatrix[0].size() - 1 && !visited[y + 1][x + 1] && logicMatrix[y + 1][x + 1] != 0) {
						visited[y + 1][x + 1] = true;
						Q.push(x + 1); Q.push(y + 1);
					}
					if ((x + 1) < logicMatrix[0].size() - 1 && !visited[y][x + 2] && logicMatrix[y][x + 2] != 0) {
						visited[y][x + 2] = true;
						Q.push(x + 2); Q.push(y);
					}
					if (x > 1 && !visited[y][x - 2] && logicMatrix[y][x - 2] != 0) {
						visited[y][x - 2] = true;
						Q.push(x - 2); Q.push(y);
					}
				}
				if (!hanging) {
					mustExplode.push(j); mustExplode.push(i); mustExplode.push(logicMatrix[i][j]);
					eraseInMap.push(j); eraseInMap.push(i);
					logicMatrix[i][j] = 0;
					map[logicToMapMatrix[i][j]] = 0;
				}
			}
		}		
	}	
}

queue<int> TileMap::getMustExplode()
{
	return mustExplode;
}


void TileMap::resetMustExplode()
{
	mustExplode = queue<int>();
}

vector<vector<int>> TileMap::getLogicMatrix()
{
	return logicMatrix;
}

int TileMap::screenToTileCellContent(glm::vec2 screenPos, int xDir)
{
	float offsetH = OFFSET_H;
	float offsetV = OFFSET_V;
	int yPos = (screenPos.y - offsetV - minCoordsRedraw.y) / tileSize;
	if (yPos % 2 == 1 - (lineOffset % 2)) offsetH -= 13.f;
	int xPos = (screenPos.x - offsetH - minCoordsRedraw.x) / tileSize;
	if (yPos >= mapSize.y) return 0;
	if (xPos >= mapSize.x) return 0;
	if (xPos <= 0) return 0;
 	return map[yPos * mapSize.x + xPos];
}

void TileMap::insertBall(glm::vec2 position, int xDir, int color)
{
	float offsetH = OFFSET_H;
	float offsetV = OFFSET_V;
	int yPos = (position.y - offsetV - minCoordsRedraw.y) / tileSize;
	if (yPos % 2 == 1 - (lineOffset % 2)) offsetH -= 13.f;
	int xPos = (position.x - offsetH - minCoordsRedraw.x) / tileSize;
	int mapPos = yPos * mapSize.x + xPos;
	
   	map[mapPos] = color + 1;
	glm::vec2 logicPos;
	checkExplosions(glm::vec2(mapToLogicMatrix[mapPos * 2], mapToLogicMatrix[mapPos * 2 + 1]), color + 1);
	this->prepareArrays(minCoordsRedraw, programRedraw);	
	this->render();
}


bool TileMap::update(int deltaTime)
{
	pixelOffset += PIXEL_MOVE_PER_FRAME * deltaTime;	
	bool result = false;
	if (pixelOffset >= tileSize) {
		lineOffset += 1;
		pixelOffset = 0;
		for (int i = (mapSize.x * mapSize.y) - 1; i >= mapSize.x; --i) {
			map[i] = map[i - mapSize.x];
		}
		result = true;
	}
	this->prepareArrays(minCoordsRedraw, programRedraw);
	this->render();
	return result;
}

bool TileMap::checkDeath()
{
	int c = 0;
	for (int i = 0; i < mapSize.x; ++i) c += map[DEATH_LINE * mapSize.x + i];
	return (c != 0);
}


































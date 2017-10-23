#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include "dependencies\glm\glm\glm.hpp"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Sound.h"


class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render(int deltaTime);

private:
	void initShaders();
	glm::mat4 projection;
	ShaderProgram texProgram;
	float currentTime;
	Sprite *menuBackground, *menuText, *menuSelector;
	Texture menuBackgroundTex, menuTextTex, menuSelectorTex;
	Sound *menuLoop;
	float rotDirection = 1.f;
	float rotAngle = 0.f;
	int menuPos = 0;
	int keyTimer = 0;
	int musicTimer = 0;
};


#endif // _MENU_INCLUDE


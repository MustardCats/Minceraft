#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../Classes/Graphics.h"
#include "../Classes/Chunk.h"
#include "../Classes/Stage.h"
#include "../Classes/Player.h"

class App {
public:
	App();
	~App();

	bool SetUp();
	bool Start();
	bool DoFrame(float deltatime);
	bool End();

	Graphics* graphics;
	Stage* stage;
	Player* player;
};
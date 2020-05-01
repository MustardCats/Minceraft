#pragma once

#include "../Namespaces/Namespaces.h"
#include "Graphics.h"
#include "Camera.h"
#include "Stage.h"
#include "Ray.h"

class Player {
public:
	Player(Graphics* setgraphics);
	~Player();

	bool Update(float deltatime, Stage* stage);
	void DetectCollision(Stage* stage);
	void MoveHorizontal(float deltatime);
	void MoveVertical(float deltatime);

	int SearchChunks(Stage* stage, Block& block, int tx, int ty, int tz1);

	misc::tcoord chunkpos;
	misc::tcoord chunkblockpos;
	float x, y, z;
	bool debug;
	bool isRunning;
	bool isJumping;
	float vVelocity;
	float hVelocity;

	short jumpUpgrade;

	Graphics* graphics;
	Camera* camera;
	Chunk* chunk;
};
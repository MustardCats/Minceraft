#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include "Chunk.h"
#include "Graphics.h"
#include "Camera.h"

class Stage {
public:
	Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz);
	~Stage();

	bool Update(misc::tcoord& chunk, misc::tcoord& chunkblock, float deltatime);
	void MakeChunks(misc::tcoord chunk, misc::tcoord chunkblock);
	void SetChunkPointers(Chunk* chunk, bool isgenerate = false);
	Chunk* FindChunk(int x, int y, int z);
	void Render();
	int SearchChunks(Block& block, int tx, int ty, int tz, misc::chunkindex* index);

	Graphics* graphics;
	int id;
	short length, width, height;
	std::vector<Chunk*> chunks;

	misc::tcoord prevchunk;
	// When the player isn't moving. More widespread.
	bool passivegen = true;
	bool activegen = true;
	bool doing;

	int cx, cy, cz;
};
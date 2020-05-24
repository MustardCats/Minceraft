#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "Chunk.h"
#include "Graphics.h"
#include "Camera.h"

class Stage {
public:
	Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz);
	~Stage();

	void MakeChunk(int x, int y, int z);

	bool Update(misc::tcoord& chunk, misc::tcoord& chunkblock, float deltatime);
	void MakeChunks(misc::tcoord* chunk);
	void SetChunkPointers(Chunk* chunk, bool isgenerate = false);

	Chunk* FindChunk(int x, int y, int z);

	void Render();

	int SearchChunks(Block& block, int tx, int ty, int tz, misc::chunkindex* index);

	Graphics* graphics;
	int id;
	short length, width, height;
	std::vector<Chunk*> chunks;

	// Annoying static variables I had to place here
	// because the whiny std::thread would error
	// "invoke" shit
	misc::tcoord prevchunk;
	// When the player isn't moving. More widespread.
	bool passivegen = true;
	bool activegen = true;
	bool doing;

	int cx, cy, cz;
};
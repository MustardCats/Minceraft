#pragma once

#include "Chunk.h"
#include "Graphics.h"

class Stage {
public:
	Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz);
	~Stage();

	bool Update(misc::tcoord& chunk, misc::tcoord& chunkblock, float deltatime);
	void MoveChunks(misc::tcoord& chunk, misc::tcoord& chunkblock);
	void SetChunkPointers(Chunk* chunk);

	Chunk* FindChunk(int x, int y, int z);

	int SearchChunks(Block& block, int tx, int ty, int tz, misc::chunkindex* index);

	Graphics* graphics;
	int id;
	short length, width, height;
	std::vector<Chunk*> chunks;
};
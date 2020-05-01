#include "../Classes/Stage.h"

Stage::Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz) {
	graphics = setgraphics;
	length = 5;
	width = 5;
	height = 3;
	id = 0;
	// Create chunks
	for (int cx = -2; cx < length - 2; cx++) {
		for (int cy = -2; cy < height - 2; cy++) {
			for (int cz = -2; cz < width - 2; cz++) {
				chunks.push_back(new Chunk(graphics, cx, cy, cz));
			}
		}
	}

	for (int i = 0; i < chunks.size(); i++) {
		chunks.at(i)->otherchunks.top = FindChunk(chunks.at(i)->x, chunks.at(i)->y + 1, chunks.at(i)->z);
		chunks.at(i)->otherchunks.bottom = FindChunk(chunks.at(i)->x, chunks.at(i)->y - 1, chunks.at(i)->z);
		chunks.at(i)->otherchunks.left = FindChunk(chunks.at(i)->x - 1, chunks.at(i)->y, chunks.at(i)->z);
		chunks.at(i)->otherchunks.right = FindChunk(chunks.at(i)->x + 1, chunks.at(i)->y, chunks.at(i)->z);
		chunks.at(i)->otherchunks.back = FindChunk(chunks.at(i)->x, chunks.at(i)->y, chunks.at(i)->z - 1);
		chunks.at(i)->otherchunks.forward = FindChunk(chunks.at(i)->x, chunks.at(i)->y, chunks.at(i)->z + 1);
		chunks.at(i)->SetVertices();
	}
}

Stage::~Stage() {

}

bool Stage::Update(misc::tcoord& chunk, misc::tcoord& chunkblock, float deltatime) {
	//MoveChunks(chunk, chunkblock);
	return true;
}

void Stage::MoveChunks(misc::tcoord& chunk, misc::tcoord& chunkblock) {
	static misc::tcoord prevchunk;
	static misc::tcoord prevchunkblock;
	static bool firsttime = true;
	if (firsttime) {
		firsttime = false;
		prevchunk = chunk;
		prevchunkblock = chunkblock;
	}
	// Move positive x
	if (chunk.x > prevchunk.x) {
		// Find minimum x
		int target = chunks.at(0)->x;
		
		for (int i = 0; i < chunks.size(); i++) {
			if (target > chunks.at(i)->x)
				target = chunks.at(i)->x;
		}
		// Move chunks
		for (int i = 0; i < chunks.size(); i++) {
			if (chunks.at(i)->x == target) {
				chunks.at(i)->MoveX(length);
				chunks.at(i)->Generate();
				SetChunkPointers(chunks.at(i));
				chunks.at(i)->SetVertices();
			}
		}
		// Reset prevchunk and block
		prevchunk = chunk;
		prevchunkblock = chunkblock;
	}
}

void Stage::SetChunkPointers(Chunk* chunk) {
	chunk->otherchunks.top = FindChunk(chunk->x, chunk->y + 1, chunk->z);
	chunk->otherchunks.bottom = FindChunk(chunk->x, chunk->y - 1, chunk->z);
	chunk->otherchunks.left = FindChunk(chunk->x - 1, chunk->y, chunk->z);
	chunk->otherchunks.right = FindChunk(chunk->x + 1, chunk->y, chunk->z);
	chunk->otherchunks.back = FindChunk(chunk->x, chunk->y, chunk->z - 1);
	chunk->otherchunks.forward = FindChunk(chunk->x, chunk->y, chunk->z + 1);
}

Chunk* Stage::FindChunk(int x, int y, int z) {
	int index;
	for (int i = 0; i < chunks.size(); i++) {
		if (chunks.at(i)->x == x && chunks.at(i)->y == y && chunks.at(i)->z == z) {
			return chunks.at(i);
		}
	}
	return nullptr;
}

int Stage::SearchChunks(Block& block, int tx, int ty, int tz, misc::chunkindex* index) {
	misc::tcoord localchunk;

	localchunk.x = ceil(tx / misc::chunkSize.x);
	localchunk.y = ceil(ty / misc::chunkSize.y);
	localchunk.z = ceil(tz / misc::chunkSize.z);

	if (tx < 0)
		localchunk.x--;
	if (ty < 0)
		localchunk.y--;
	if (tz < 0)
		localchunk.z--;

	misc::tcoord localblock;
	for (int i = 0; i < chunks.size(); i++) {
		if (chunks.at(i)->x == localchunk.x
			|| chunks.at(i)->y == localchunk.y
			|| chunks.at(i)->z == localchunk.z) {
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (chunks.at(i)->blocks.at(cx).at(0).at(0).x == tx) {
					localblock.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (chunks.at(i)->blocks.at(0).at(cy).at(0).y == ty) {
							localblock.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (chunks.at(i)->blocks.at(cx).at(cy).at(cz).z == tz
									&& chunks.at(i)->blocks.at(cx).at(cy).at(cz).x == tx
									&& chunks.at(i)->blocks.at(cx).at(cy).at(cz).y == ty) {
									/*
									std::cout << "Found block " << chunks.at(i)->blocks.at(cx).at(cy).at(cz).id <<
										" at " << chunks.at(i)->blocks.at(cx).at(cy).at(cz).x << " "
										<< chunks.at(i)->blocks.at(cx).at(cy).at(cz).y << " "
										<< chunks.at(i)->blocks.at(cx).at(cy).at(cz).z << "\n";*/
									block = chunks.at(i)->blocks.at(cx).at(cy).at(cz);
									if (index != nullptr) {
										index->c = i;
										index->bx = cx;
										index->by = cy;
										index->bz = cz;
									}
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
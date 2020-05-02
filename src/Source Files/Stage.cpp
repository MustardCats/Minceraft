#include "../Classes/Stage.h"

Stage::Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz) {
	graphics = setgraphics;
	length = 5;
	width = 5;
	height = 3;
	id = 0;
	// Create chunks
	for (int cx = chunkx - (length / 2); cx < chunkx + ceil((float)length / 2.0); cx++) {
		for (int cy = chunky - (height / 2); cy < chunky + ceil((float)height / 2.0); cy++) {
			for (int cz = chunkz - (width / 2); cz < chunkz + ceil((float)width / 2.0); cz++) {
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
	MoveChunks(chunk, chunkblock);
	return true;
}

void Stage::MoveChunks(misc::tcoord& chunk, misc::tcoord& chunkblock) {
	static misc::tcoord prevchunk = chunk;
	static misc::tcoord prevchunkblock = chunkblock;

	static bool movepx = false;
	static bool movenx = false;
	static bool movepy = false;
	static bool moveny = false;
	static bool movepz = false;
	static bool movenz = false;
	// positive x
	if ((chunk.x > prevchunk.x || movepx) && !(movepy || moveny || movepz || movenz)) {
		std::cout << "moving chunks!\n";
		// the index within the indices! must be reset.
		static int indexwithin = 0;
		
		// Find minimum x
		static int target = chunks.at(0)->x;
		
		if (!movepx) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target > chunks.at(i)->x) {
					target = chunks.at(i)->x;
					std::cout << "yes: " << i << "\n";
				}
			}
		}
		// Move chunks

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->x == target) {
				std::cout << "Moving chunk at x " << target << " length " << length << "\n";
				chunks.at(index)->MoveX(length);
				chunks.at(index)->Generate();
				indices.push_back(index);
				movepx = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && movepx) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}
				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				movepx = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->x;
				// reset indices
				indices.clear();
				std::cout << "finished moving these chunks p!\n";
			}
		}
		// Reset prevchunk and block
		prevchunk.x = chunk.x;
		prevchunkblock.x = chunkblock.x;
	}
	// negative x
	if ((chunk.x < prevchunk.x || movenx) && !(movepy || moveny || movepz || movenz)) {
		std::cout << "moving chunks!\n";
		// the index within the indices! must be reset.
		static int indexwithin = 0;

		// Find minimum x
		static int target = chunks.at(0)->x;

		if (!movenx) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target < chunks.at(i)->x) {
					target = chunks.at(i)->x;
					std::cout << "yes: " << i << "\n";
				}
			}
		}
		// Move chunks

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->x == target) {
				std::cout << "Moving chunk at x " << target << " length " << length << "\n";
				chunks.at(index)->MoveX(-length);
				chunks.at(index)->Generate();
				indices.push_back(index);
				movenx = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && movenx) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update top chunk
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}
				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				movenx = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->x;
				// reset indices
				indices.clear();
				std::cout << "finished moving these chunks n!\n";
			}
		}
		// Reset prevchunk and block
		prevchunk.x = chunk.x;
		prevchunkblock.x = chunkblock.x;
	}
	// positive y
	if ((chunk.y > prevchunk.y || movepy) && !(movepx || movenx || movepz || movenz)) {
		// the index within the indices! must be reset.
		static int indexwithin = 0;

		// Find minimum x
		static int target = chunks.at(0)->y;

		if (!movepy) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target > chunks.at(i)->y) {
					target = chunks.at(i)->y;
				}
			}
		}
		// Move chunks

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->y == target) {
				chunks.at(index)->MoveY(height);
				chunks.at(index)->Generate();
				indices.push_back(index);
				movepy = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && movepy) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update top chunk
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				movepy = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->y;
				// reset indices
				indices.clear();
				std::cout << "finished moving these chunks!\n";
			}
		}
		// Reset prevchunk and block
		prevchunk.y = chunk.y;
		prevchunkblock.y = chunkblock.y;
	}
	// negative y
	if ((chunk.y < prevchunk.y || moveny) && !(movepx || movenx || movepz || movenz)) {
		// the index within the indices! must be reset.
		static int indexwithin = 0;

		// Find minimum x
		static int target = chunks.at(0)->y;

		if (!moveny) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target < chunks.at(i)->y) {
					target = chunks.at(i)->y;
				}
			}
		}
		// Move chunks

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->y == target) {
				chunks.at(index)->MoveY(-height);
				chunks.at(index)->Generate();
				indices.push_back(index);
				moveny = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && moveny) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update top chunk
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				std::cout << "finished moving these chunks to " << chunks.at(indices.at(indexwithin - 1))->y << "\n";
				moveny = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->y;
				// reset indices
				indices.clear();
			}
		}
		// Reset prevchunk and block
		prevchunk.y = chunk.y;
		prevchunkblock.y = chunkblock.y;
	}

	// positive z
	if ((chunk.z > prevchunk.z || movepz) && !(movepy || moveny || movepx || movenx)) {
		// the index within the indices! must be reset.
		static int indexwithin = 0;

		// Find minimum x
		static int target = chunks.at(0)->z;

		if (!movepz) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target > chunks.at(i)->z) {
					target = chunks.at(i)->z;
				}
			}
		}
		// Move chunks 

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->z == target) {
				chunks.at(index)->MoveZ(width);
				chunks.at(index)->Generate();
				indices.push_back(index);
				movepz = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && movepz) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update top chunk
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				movepz = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->z;
				// reset indices
				indices.clear();
				std::cout << "finished moving these chunks!\n";
			}
		}
		// Reset prevchunk and block
		prevchunk.z = chunk.z;
		prevchunkblock.z = chunkblock.z;
	}
	// negative z
	if ((chunk.z < prevchunk.z || movenz) && !(movepy || moveny || movepx || movenx)) {
		// the index within the indices! must be reset.
		static int indexwithin = 0;

		// Find minimum x
		static int target = chunks.at(0)->z;

		if (!movenz) {
			for (int i = 0; i < chunks.size(); i++) {
				if (target < chunks.at(i)->z) {
					target = chunks.at(i)->z;
				}
			}
		}
		// Move chunks

		// index for the chunk
		static short index = 0;
		// vector of indices of chunks moved
		static std::vector<short> indices;
		while (index < chunks.size()) {
			if (chunks.at(index)->z == target) {
				chunks.at(index)->MoveZ(-width);
				chunks.at(index)->Generate();
				indices.push_back(index);
				movenz = true;
				index++;
				break;
			}
			else
				index++;
		}
		// When moving is over, make new vertex and uv buffers
		if (index == chunks.size() && movenz) {
			if (indexwithin < indices.size()) {
				SetChunkPointers(chunks.at(indices.at(indexwithin)));
				// Update top chunk
				for (int j = 0; j < chunks.size(); j++) {
					SetChunkPointers(chunks.at(j));
				}				chunks.at(indices.at(indexwithin))->SetVertices();
				indexwithin++;
			}
			else {
				std::cout << "finished moving these chunks to " << chunks.at(indices.at(indexwithin - 1))->z << "\n";
				movenz = false;
				index = 0;
				indexwithin = 0;
				target = chunks.at(0)->z;
				// reset indices
				indices.clear();
			}
		}
		// Reset prevchunk and block
		prevchunk.z = chunk.z;
		prevchunkblock.z = chunkblock.z;
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
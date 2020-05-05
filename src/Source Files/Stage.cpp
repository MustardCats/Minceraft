#include "../Classes/Stage.h"

Stage::Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz) {
	graphics = setgraphics;
	length = 7;
	width = 7;
	height = 5;
	id = 0;
	doing = false;
	// Create chunks
	for (int cx = chunkx - (length / 2); cx <= chunkx + (length / 2); cx++) {
		for (int cy = chunky - (height / 2); cy <= chunky + (height / 2); cy++) {
			for (int cz = chunkz - (width / 2); cz <= chunkz + (width / 2); cz++) {
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

	auto start = std::chrono::high_resolution_clock::now();
	//MakeChunk(0, 0, 0);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "time: " << duration.count() << "\n";
}

Stage::~Stage() {

}

void Stage::MakeChunk(int x, int y, int z) {
	chunks.push_back(new Chunk(graphics, x, y, z));
	chunks.at(chunks.size() - 1)->Generate();
	SetChunkPointers(chunks.at(chunks.size() - 1));
	chunks.at(chunks.size() - 1)->SetVertices();
	// Update surrounding chunks' pointers
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.top);
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.bottom);
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.back);
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.forward);
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.left);
	SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.right);
	// Set the new vertices
	if (chunks.at(chunks.size() - 1)->otherchunks.top != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.top->SetVertices();
	if (chunks.at(chunks.size() - 1)->otherchunks.bottom != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.bottom->SetVertices();
	if (chunks.at(chunks.size() - 1)->otherchunks.left != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.left->SetVertices();
	if (chunks.at(chunks.size() - 1)->otherchunks.right != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.right->SetVertices();
	if (chunks.at(chunks.size() - 1)->otherchunks.back != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.back->SetVertices();
	if (chunks.at(chunks.size() - 1)->otherchunks.forward != nullptr)
		chunks.at(chunks.size() - 1)->otherchunks.forward->SetVertices();
}

bool Stage::Update(misc::tcoord& chunk, misc::tcoord& chunkblock, float deltatime) {
	misc::tcoord stuff;
	stuff.x = 0;
	stuff.y = 0;
	stuff.z = 0;

	//MakeChunks(&chunk);


	return true;
}



void Stage::MakeChunks(misc::tcoord* chunk) {
	doing = true;
	prevchunk = *chunk;
	if (passivegen) {
		cx = chunk->x - (length / 2);
		cy = chunk->y - (height / 2);
		cz = chunk->z - (width / 2);
		// If passivegen is just starting again
		cx = chunk->x - (length / 2);
		cy = chunk->y - (height / 2);
		cz = chunk->z - (width / 2);
		// Check whether a chunk is nullptr
		bool exit = false;
		
		while (FindChunk(cx, cy, cz) != nullptr && !exit) {
			if (cx < chunk->x + (length / 2))
				cx++;
			else {
				cx = chunk->x - (length / 2);
				if (cy < chunk->y + (height / 2))
					cy++;
				else {
					cy = chunk->y - (height / 2);
					if (cz < chunk->z + (width / 2))
						cz++;
					else {
						exit = true;
						break;
					}
				}
			}
		}




		// Make new chunk
		if (FindChunk(chunk->x, chunk->y, chunk->z) == nullptr) {
			chunks.push_back(new Chunk(graphics, chunk->x, chunk->y, chunk->z));
			SetChunkPointers(chunks.at(chunks.size() - 1), true);
		}
		if (FindChunk(cx, cy, cz) == nullptr && !exit) {
			//std::cout << "Creating chunk at " << cx << " " << cy << " " << cz << "\n";
			

			chunks.push_back(new Chunk(graphics, cx, cy, cz));
			
			SetChunkPointers(chunks.at(chunks.size() - 1), true);
			
			
			// update other chunks
			if (chunks.at(chunks.size() - 1)->otherchunks.top != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.top, true);
			}
			if (chunks.at(chunks.size() - 1)->otherchunks.bottom != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.bottom, true);
			}
			if (chunks.at(chunks.size() - 1)->otherchunks.back != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.back, true);
			}
			if (chunks.at(chunks.size() - 1)->otherchunks.forward != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.forward, true);
			}
			if (chunks.at(chunks.size() - 1)->otherchunks.left != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.left, true);
			}
			if (chunks.at(chunks.size() - 1)->otherchunks.right != nullptr) {
				SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.right, true);
			}
		}
		// Delete chunks
		misc::tcoord distance;
		for (int i = 0; i < chunks.size(); i++) {
			distance.x = abs(chunk->x - chunks.at(i)->x);
			distance.y = abs(chunk->y - chunks.at(i)->y);
			distance.z = abs(chunk->z - chunks.at(i)->z);

			if (distance.x > length || distance.y > height || distance.z > width) {
				std::cout << "delete\n";
				delete chunks.at(i);
				chunks.erase(chunks.begin() + i);
			}
		}
	}
	else {
		activegen = true;
	}
	doing = false;
}

void Stage::SetChunkPointers(Chunk* chunk, bool isgenerate) {
	chunk->otherchunks.top = FindChunk(chunk->x, chunk->y + 1, chunk->z);
	chunk->otherchunks.bottom = FindChunk(chunk->x, chunk->y - 1, chunk->z);
	chunk->otherchunks.left = FindChunk(chunk->x - 1, chunk->y, chunk->z);
	chunk->otherchunks.right = FindChunk(chunk->x + 1, chunk->y, chunk->z);
	chunk->otherchunks.back = FindChunk(chunk->x, chunk->y, chunk->z - 1);
	chunk->otherchunks.forward = FindChunk(chunk->x, chunk->y, chunk->z + 1);
	if (isgenerate)
		chunk->SetVertices();
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
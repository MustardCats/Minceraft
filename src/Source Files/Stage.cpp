#include "../Classes/Stage.h"

Stage::Stage(Graphics* setgraphics, int chunkx, int chunky, int chunkz) {
	graphics = setgraphics;
	length = 1;
	width = 1;
	height = 1;
	id = 0;
	doing = false;

	chunks.push_back(new Chunk(graphics, 0, 0, 0));
	for (int i = 0; i < 16; i++) {
		chunks.at(0)->SetVertices();
	}
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
	// temp
	static bool once = true;

	misc::tcoord stuff;
	stuff.x = 0;
	stuff.y = 0;
	stuff.z = 0;

	int bx, by, bz;
	bool exit = false;
	bx = chunk.x - (length / 2);
	by = chunk.y - (height / 2);
	bz = chunk.z - (width / 2);
	// Check nearby chunks (manual for now)
	if (true) {
		if (FindChunk(bx + 1, by, bz) == nullptr) {
			exit = true;
			bx++;
		}
		else if (FindChunk(bx - 1, by, bz) == nullptr) {
			exit = true;
			bx--;
		}
		else if (FindChunk(bx, by + 1, bz) == nullptr) {
			exit = true;
			by++;
		}
		else if (FindChunk(bx, by - 1, bz) == nullptr) {
			exit = true;
			by--;
		}
		else if (FindChunk(bx, by, bz + 1) == nullptr) {
			exit = true;
			bz++;
		}
		else if (FindChunk(bx, by, bz - 1) == nullptr) {
			exit = true;
			bz--;
		}
	}
	// Do other chunks
	while (bx < chunk.x + (length / 2) && !exit) {
		by = chunk.y - (height / 2);
		while (by < chunk.y + (height / 2)) {
			bz = chunk.z - (width / 2);
			while (bz < chunk.z + (width / 2)) {
				if (FindChunk(bx, by, bz) == nullptr) {
					exit = true;
					break;
				}
				bz++;
			}
			if (exit)
				break;
			by++;
		}
		if (exit)
			break;
		bx++;
	}
	if (exit && once) {
		//once = false;
		chunks.push_back(new Chunk(graphics, bx, by, bz));
		SetChunkPointers(chunks.at(chunks.size() - 1));
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.back);
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.bottom);
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.forward);
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.left);
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.right);
		SetChunkPointers(chunks.at(chunks.size() - 1)->otherchunks.top);
		// Start updating nearby chunks so there are no invisible spots
		if (chunks.at(chunks.size() - 1)->otherchunks.back != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.back->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.back->bufferloaded = false;
		}
		if (chunks.at(chunks.size() - 1)->otherchunks.bottom != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.bottom->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.bottom->bufferloaded = false;
		}
		if (chunks.at(chunks.size() - 1)->otherchunks.left != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.left->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.left->bufferloaded = false;
		}
		if (chunks.at(chunks.size() - 1)->otherchunks.right != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.right->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.right->bufferloaded = false;
		}
		if (chunks.at(chunks.size() - 1)->otherchunks.forward != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.forward->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.forward->bufferloaded = false;
		}
		if (chunks.at(chunks.size() - 1)->otherchunks.top != nullptr) {
			chunks.at(chunks.size() - 1)->otherchunks.top->firsttime = true;
			chunks.at(chunks.size() - 1)->otherchunks.top->bufferloaded = false;
		}
	}
	else if (once) {
		//once = false;
		static int i = 0;
		if (i < chunks.size()) {
			int count = 0;
			while (!chunks.at(i)->bufferloaded && count < 16 &&
				!((chunks.at(i)->x < chunk.x - (length / 2) ||
				chunks.at(i)->x > chunk.x + (length / 2) ||
				chunks.at(i)->y < chunk.y - (height / 2) ||
				chunks.at(i)->y > chunk.y + (height / 2) ||
				chunks.at(i)->z < chunk.z - (width / 2) ||
				chunks.at(i)->z > chunk.z + (width / 2)))) {
				chunks.at(i)->SetVertices();
				count++;
			}
			i++;
		}
		else {
			i = 0;
			// Delete unneeded chunks
			for (int j = 0; j < chunks.size(); j++) {
				if (chunks.at(j) != nullptr) {
					if (chunks.at(j)->x < chunk.x - (length / 2) ||
						chunks.at(j)->x > chunk.x + (length / 2) ||
						chunks.at(j)->y < chunk.y - (height / 2) ||
						chunks.at(j)->y > chunk.y + (height / 2) ||
						chunks.at(j)->z < chunk.z - (width / 2) ||
						chunks.at(j)->z > chunk.z + (width / 2)) {
						delete chunks.at(j);
						chunks.erase(chunks.begin() + j);
					}
				}
			}
		}
	}
	// Delete nullptrs
	for (int i = 0; i < chunks.size(); i++) {
		if (chunks.at(i) == nullptr)
			chunks.erase(chunks.begin() + i);
	}

	return true;
}

void Stage::MakeChunks(misc::tcoord* chunk) {
	
}

void Stage::SetChunkPointers(Chunk* chunk, bool isgenerate) {
	if (chunk != nullptr) {
		chunk->otherchunks.top = FindChunk(chunk->x, chunk->y + 1, chunk->z);
		chunk->otherchunks.bottom = FindChunk(chunk->x, chunk->y - 1, chunk->z);
		chunk->otherchunks.left = FindChunk(chunk->x - 1, chunk->y, chunk->z);
		chunk->otherchunks.right = FindChunk(chunk->x + 1, chunk->y, chunk->z);
		chunk->otherchunks.back = FindChunk(chunk->x, chunk->y, chunk->z - 1);
		chunk->otherchunks.forward = FindChunk(chunk->x, chunk->y, chunk->z + 1);
		if (isgenerate)
			chunk->SetVertices();
	}
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

void Stage::Render() {
	std::vector<GLfloat> totalvertex;
	std::vector<GLfloat> totaluv;

	for (int i = 0; i < chunks.size(); i++) {
		totalvertex.insert(totalvertex.end(), chunks.at(i)->vertexdata.begin(), chunks.at(i)->vertexdata.end());
		totaluv.insert(totaluv.end(), chunks.at(i)->uvdata.begin(), chunks.at(i)->uvdata.end());
	}

	glUseProgram(graphics->colorProgramID);

	GLuint vertexbuffer;
	GLuint colorbuffer;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->sheet1->texture);
	glUniform1i(graphics->sheet1->TextureID, 0);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * totalvertex.size(), totalvertex.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * totaluv.size(), totaluv.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, totalvertex.size());

	//glDeleteProgram(colorProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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
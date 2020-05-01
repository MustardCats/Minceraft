#include "../Classes/Chunk.h"

Chunk::Chunk(Graphics* setgraphics, int setx, int sety, int setz) {
	x = setx;
	y = sety;
	z = setz;
	//std::cout << "Making chunk " << x << " " << y << " " << z << "\n";
	int xfactor, yfactor, zfactor;
	graphics = setgraphics;
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		blocks.push_back(std::vector<std::vector<Block>>());
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			blocks.at(cx).push_back(std::vector<Block>());
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				xfactor = (misc::chunkSize.x * x) + cx;
				yfactor = (misc::chunkSize.y * y) + cy;
				zfactor = (misc::chunkSize.z * z) + cz;
				blocks.at(cx).at(cy).push_back(Block(xfactor, yfactor, zfactor, 1));
			}
		}
	}
	Generate();
}

void Chunk::BeginLoading() {

}

void Chunk::KeepLoading() {

}
/*
x - 0.5f, y - 0.5f, z - 0.5f, // forward down left
x - 0.5f, y - 0.5f, z + 0.5f, // forward down right
x - 0.5f, y + 0.5f, z + 0.5f, // forward up right

// up front face
x - 0.5f, y - 0.5f, z - 0.5f, // forward down left
x - 0.5f, y + 0.5f, z + 0.5f, // forward up right
x - 0.5f, y + 0.5f, z - 0.5f, // forward up left
*/
void Chunk::SetVertices() {
	//std::cout << "Setting buffer data at " << x << " " << y << " " << z << "\n";
	vertexdata.clear();
	uvdata.clear();
	//std::cout << "Buffer data cleared\n";
	//std::cout << "Setting vertex buffers at " << x << " " << y << " " << z << "\n";
	misc::uvcoords uvcoords[6];
	// Whether or not buffers will be made
	bool yes = true;
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				if (blocks.at(cx).at(cy).at(cz).id != 0) {
					switch (blocks.at(cx).at(cy).at(cz).id) {
					case 1:
						uvcoords[0] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[1] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[2] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[3] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[4] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[5] = graphics->sheet1->GetUvCoords(0, 0);
						break;
					case 2:
						uvcoords[0] = graphics->sheet1->GetUvCoords(0, 1);
						uvcoords[1] = graphics->sheet1->GetUvCoords(0, 1);
						uvcoords[2] = graphics->sheet1->GetUvCoords(0, 0);
						uvcoords[3] = graphics->sheet1->GetUvCoords(1, 0);
						uvcoords[4] = graphics->sheet1->GetUvCoords(0, 1);
						uvcoords[5] = graphics->sheet1->GetUvCoords(0, 1);
						break;
					}
					// GOOD GOOD
					yes = true;
					if (cx != 0) {
						if (blocks.at(cx - 1).at(cy).at(cz).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.left != nullptr) {
						if (otherchunks.left->blocks.at(15).at(cy).at(cz).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						uvdata.insert(uvdata.end(), {
							// forward
							uvcoords[0].dl.x, uvcoords[0].dl.y,
							uvcoords[0].dr.x, uvcoords[0].dr.y,
							uvcoords[0].ur.x, uvcoords[0].ur.y,

							uvcoords[0].dl.x, uvcoords[0].dl.y,
							uvcoords[0].ur.x, uvcoords[0].ur.y,
							uvcoords[0].ul.x, uvcoords[0].ul.y,
							});
					}
					// GOOD GOOD
					yes = true;
					if (cx != 15) {
						if (blocks.at(cx + 1).at(cy).at(cz).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.right != nullptr) {
						if (otherchunks.right->blocks.at(0).at(cy).at(cz).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						uvdata.insert(uvdata.end(), {
							uvcoords[1].dl.x, uvcoords[1].dl.y,
							uvcoords[1].dr.x, uvcoords[1].dr.y,
							uvcoords[1].ur.x, uvcoords[1].ur.y,

							uvcoords[1].ul.x, uvcoords[1].ul.y,
							uvcoords[1].dl.x, uvcoords[1].dl.y,
							uvcoords[1].ur.x, uvcoords[1].ur.y,
							});
					}
					// GOOD GOOD
					yes = true;
					if (cy != 0) {
						if (blocks.at(cx).at(cy - 1).at(cz).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.bottom != nullptr) {
						if (otherchunks.bottom->blocks.at(cx).at(15).at(cz).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						uvdata.insert(uvdata.end(), {
							uvcoords[2].ul.x, uvcoords[2].ul.y,
							uvcoords[2].ur.x, uvcoords[2].ur.y,
							uvcoords[2].dl.x, uvcoords[2].dl.y,

							uvcoords[2].ur.x, uvcoords[2].ur.y,
							uvcoords[2].dr.x, uvcoords[2].dr.y,
							uvcoords[2].dl.x, uvcoords[2].dl.y,
							});
					}
					// GOOD GOOD
					yes = true;
					if (cy != 15) {
						if (blocks.at(cx).at(cy + 1).at(cz).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.top != nullptr) {
						if (otherchunks.top->blocks.at(cx).at(0).at(cz).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);

						uvdata.insert(uvdata.end(), {
							uvcoords[3].ul.x, uvcoords[3].ul.y,
							uvcoords[3].dl.x, uvcoords[3].dl.y,
							uvcoords[3].ur.x, uvcoords[3].ur.y,

							uvcoords[3].dl.x, uvcoords[3].dl.y,
							uvcoords[3].dr.x, uvcoords[3].dr.y,
							uvcoords[3].ur.x, uvcoords[3].ur.y,
							});
					}
					// GOOD
					yes = true;
					if (cz != 0) {
						if (blocks.at(cx).at(cy).at(cz - 1).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.back != nullptr) {
						if (otherchunks.back->blocks.at(cx).at(cy).at(15).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z - 0.5f);

						uvdata.insert(uvdata.end(), {
							uvcoords[4].dl.x, uvcoords[4].dl.y,
							uvcoords[4].dr.x, uvcoords[4].dr.y,
							uvcoords[4].ur.x, uvcoords[4].ur.y,

							uvcoords[4].ur.x, uvcoords[4].ur.y,
							uvcoords[4].ul.x, uvcoords[4].ul.y,
							uvcoords[4].dl.x, uvcoords[4].dl.y,
							});
					}
					
					yes = true;
					if (cz != 15) {
						if (blocks.at(cx).at(cy).at(cz + 1).id != 0) {
							yes = false;
						}
					}
					else if (otherchunks.forward != nullptr) {
						if (otherchunks.forward->blocks.at(cx).at(cy).at(0).id != 0) {
							yes = false;
						}
					}
					if (yes) {
						bufferIndices.push_back(blockBufferIndex(cx, cy, cz, vertexdata.size() - 1, uvdata.size() - 1));
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);

						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y - 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).x + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).y + 0.5f);
						vertexdata.push_back(blocks.at(cx).at(cy).at(cz).z + 0.5f);

						uvdata.insert(uvdata.end(), {
							uvcoords[5].dl.x, uvcoords[5].dl.y,
							uvcoords[5].dr.x, uvcoords[5].dr.y,
							uvcoords[5].ur.x, uvcoords[5].ur.y,

							uvcoords[5].ul.x, uvcoords[5].ul.y,
							uvcoords[5].dl.x, uvcoords[5].dl.y,
							uvcoords[5].ur.x, uvcoords[5].ur.y,
							});
					}
				}
			}
		}
	}
}

void Chunk::Render() {
	glUseProgram(graphics->colorProgramID);

	GLuint vertexbuffer;
	GLuint colorbuffer;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, graphics->sheet1->texture);
	glUniform1i(graphics->sheet1->TextureID, 0);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexdata.size(), vertexdata.data(), GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvdata.size(), uvdata.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, vertexdata.size());

	//glDeleteProgram(colorProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Chunk::Generate() {
	FastNoise noise;
	noise.SetNoiseType(FastNoise::Perlin);
	// default is 0.01
	noise.SetFrequency(0.02);
	float heightmap[16][16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			heightmap[i][j] = noise.GetNoise((i + x * misc::chunkSize.x) + 1002, (j + z * misc::chunkSize.z)) * 5;
		}
	}
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				if (blocks.at(cx).at(cy).at(cz).y == (int)heightmap[cx][cz])
					blocks.at(cx).at(cy).at(cz).id = 2;
				else if (blocks.at(cx).at(cy).at(cz).y < (int)heightmap[cx][cz])
					blocks.at(cx).at(cy).at(cz).id = 1;
				else
					blocks.at(cx).at(cy).at(cz).id = 0;
			}
		}
	}
}

void Chunk::MoveX(int distance) {
	x += distance;
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				blocks.at(cx).at(cy).at(cz).x += misc::chunkSize.x * distance;
			}
		}
	}
}

void Chunk::MoveY(int distance) {
	y += distance;
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				blocks.at(cx).at(cy).at(cz).y += misc::chunkSize.y * distance;
			}
		}
	}
}

void Chunk::MoveZ(int distance) {
	z += distance;
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		for (int cy = 0; cy < misc::chunkSize.y; cy++) {
			for (int cz = 0; cz < misc::chunkSize.z; cz++) {
				blocks.at(cx).at(cy).at(cz).z += misc::chunkSize.z * distance;
			}
		}
	}
}

#include "../Classes/Chunk.h"

Chunk::Chunk(Graphics* setgraphics, int setx, int sety, int setz) {
	x = setx;
	y = sety;
	z = setz;
	//std::cout << "Making chunk " << x << " " << y << " " << z << "\n";
	misc::tcoord factor;
	graphics = setgraphics;
	bufferloaded = false;
	firsttime = true;
	generated = false;
}

Chunk::~Chunk() {
	
}

bool Chunk::InView(Camera* camera) {
	/*
	// Horizontal
	float hcangle;
	float hangle;
	hcangle = fmod(camera->horizontalAngle, M_PI * 2);
	hangle = (atan2((x * 16 + 8) - camera->position[0], (z * 16 + 8) - camera->position[2]));
	if (hangle < 0)
		hangle = (M_PI * 2) - abs(hangle);
	//std::cout << "angle: " << angle << " camera angle: " << cangle << "\n";
	// Vertical
	float vangle;
	// uses the distance formula since x and z must be combined
	vangle = atan2((y * 16 + 8) - camera->position[1],
		sqrt(pow((x * 16 + 8) - (camera->position[0]), 2) +
			pow((z * 16 + 8) - (camera->position[2]), 2)));
	//std::cout << "Chunk " << x << " " << y << " " << z << " " << hangle << " " << vangle
	//<< "Player " << hcangle << " " << camera->verticalAngle << "\n";
	// If They are in range, return true
	if ((hcangle - (M_PI / 1.5) < hangle && hcangle + (M_PI / 1.5) > hangle ||
		((hcangle + (M_PI * 2)) - (M_PI / 1.5) < hangle && (hcangle + (M_PI * 2)) + (M_PI / 1.5) > hangle) ||
		((hcangle - (M_PI * 2)) - (M_PI / 1.5) < hangle && (hcangle - (M_PI * 2)) + (M_PI / 1.5) > hangle)) &&
		camera->verticalAngle - (M_PI / 2) < vangle &&
		camera->verticalAngle + (M_PI / 2) > vangle) {
		return true;
	}
	return false;
	*/
	float distanceToPlayer; // distance between player and chunk
	float pointToPoint; // distance between the camera's point and chunk
	glm::vec3 cameraPoint; // made from camera angle + distanceToPlayer
	// distance vector
	glm::vec3 direction = glm::vec3(
		cos(camera->verticalAngle) * sin(camera->horizontalAngle),
		sin(camera->verticalAngle),
		cos(camera->verticalAngle) * cos(camera->horizontalAngle)
	);

	distanceToPlayer = sqrt(pow(((x * chunksize) + (chunksize / 2) - camera->position[0]), 2) + pow(((y * chunksize) + (chunksize / 2) - camera->position[1]), 2) + pow(((z * chunksize) + (chunksize / 2) - camera->position[2]), 2));

	cameraPoint = camera->position + (direction * distanceToPlayer);

	pointToPoint = sqrt(pow(((x * 16) + (chunksize / 2) - cameraPoint[0]), 2) + pow(((y * 16) + (chunksize / 2) - cameraPoint[1]), 2) + pow(((z * chunksize) + (chunksize / 2) - cameraPoint[2]), 2));

	//std::cout << "Distance: " << distanceToPlayer << " Point to Point: " << pointToPoint << "\n";
	//std::cout << "Point: " << cameraPoint[0] << " " << cameraPoint[1] << " " << cameraPoint[2] <<
	//	" Chunk Middle: " << (x * 16) + 8 << " " << (y * 16) + 8 << " " << (z * 16) + 8 << "\n";

	if (pointToPoint < 40) {
		return true;
	}
	return false;
}

// Removes block's buffer data
void Chunk::RemoveBlock(int bx, int by, int bz) {
	//std::cout << "start\n";
	for (int i = 0; i < bufferIndices.size(); i++) {
		if (bufferIndices.at(i).x == bx &&
			bufferIndices.at(i).y == by &&
			bufferIndices.at(i).z == bz) {
			std::cout << "Removing block at " << bx << " " << by << " " << bz << " " << i << "\n";
			// Erase mesh data
			vertexdata.erase(vertexdata.begin() + bufferIndices.at(i).vertexindex, vertexdata.begin() + bufferIndices.at(i).vertexindex + 18);
			uvdata.erase(uvdata.begin() + bufferIndices.at(i).uvindex, uvdata.begin() + bufferIndices.at(i).uvindex + 12);
			bufferIndices.erase(bufferIndices.begin() + i);
			// Update the index data
			for (int j = i; j < bufferIndices.size(); j++) {
				bufferIndices.at(j).vertexindex -= 18;
				bufferIndices.at(j).uvindex -= 12;
			}
			i = 0;
		}
	}
	//std::cout << "end\n";
}
// Updates the block's buffer data for whatever reason
bool Chunk::UpdateBlock(int cx, int cy, int cz, bool loading) {
	// edges of chunk (for testing purposes)
	const static bool edges = false;
	// Erase all previous data first
	if (!loading)
		RemoveBlock(cx, cy, cz);
	std::array<misc::uvcoords, 6> uvcoords;
	// Replace data
	uvcoords = graphics->sheet1->GetBlockCoords(blocks.at(cx).at(cy).at(cz).id);
	// GOOD GOOD
	bool yes = true;
	if (cx != 0) {
		if (blocks.at(cx - 1).at(cy).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (otherchunks.left != nullptr) {
		if (otherchunks.left->blocks.at((chunksize - 1)).at(cy).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	} 

	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
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
	if (cx != (chunksize - 1)) {
		if (blocks.at(cx + 1).at(cy).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (otherchunks.right != nullptr) {
		if (otherchunks.right->blocks.at(0).at(cy).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	}
	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

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
		if (otherchunks.bottom->blocks.at(cx).at((chunksize - 1)).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	}
	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

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
	if (cy != (chunksize - 1)) {
		if (blocks.at(cx).at(cy + 1).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (otherchunks.top != nullptr) {
		if (otherchunks.top->blocks.at(cx).at(0).at(cz).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	}
	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);

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
		if (otherchunks.back->blocks.at(cx).at(cy).at((chunksize - 1)).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	}
	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) - 0.5f);

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
	if (cz != (chunksize - 1)) {
		if (blocks.at(cx).at(cy).at(cz + 1).id != 0) {
			yes = false;
		}
	}
	else if (otherchunks.forward != nullptr) {
		if (otherchunks.forward->blocks.at(cx).at(cy).at(0).id != 0) {
			yes = false;
		}
	}
	else if (!edges) {
		yes = false;
	}
	if (yes) {
		bufferIndices.push_back(blockBufferIndex(cx, cy, cz,
			vertexdata.size(), uvdata.size()));

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);

		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) - 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) - 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);
		vertexdata.push_back(((x * chunksize) + cx) + 0.5f);
		vertexdata.push_back(((y * chunksize) + cy) + 0.5f);
		vertexdata.push_back(((z * chunksize) + cz) + 0.5f);

		uvdata.insert(uvdata.end(), {
			uvcoords[5].dl.x, uvcoords[5].dl.y,
			uvcoords[5].dr.x, uvcoords[5].dr.y,
			uvcoords[5].ur.x, uvcoords[5].ur.y,

			uvcoords[5].ul.x, uvcoords[5].ul.y,
			uvcoords[5].dl.x, uvcoords[5].dl.y,
			uvcoords[5].ur.x, uvcoords[5].ur.y,
			});
	}
	return true;
}

void Chunk::SetVertices(int number) {
	// The loop allows the spreading of loading the mesh
	for (int i = 0; i < number && cx < chunksize; i++) {
		if (firsttime) {
			cx = 0;
			vertexdata.clear();
			uvdata.clear();
			bufferIndices.clear();
			firsttime = false;
		}
		if (cx < chunksize) {
			for (int cy = 0; cy < chunksize; cy++) {
				for (int cz = 0; cz < chunksize; cz++) {
					try {
						// If 0 is included, then textures will be everywhere in the air
						if (blocks.at(cx).at(cy).at(cz).id != 0) {
							UpdateBlock(cx, cy, cz, true);
						}
					}
					// Back when the chunk's pointers that pointed to wrong data weren't reset
					//  when that adjacent chunk was deleted, this was used to catch errors
					catch (const std::out_of_range& e) {
						std::cout << cx << " " << cy << " " << cz << "\n";
						std::cout << "Chunk: " << x << " " << y << " " << z << "\n";
						bufferloaded = true;
						cy = chunksize;
						cz = chunksize;
						cx = chunksize;
					}
				}
			}
		}
		if (cx == chunksize - 1) {
			cx = 0;
			bufferloaded = true;
			firsttime = true;
		}
		cx++;
	}
}

void Chunk::Render() {
	// Only use after rendering with a different program
	if (graphics->rendermode == SHAPE) {
		glUseProgram(graphics->colorProgramID);
		graphics->rendermode = TEXTURE;
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(graphics->sheet1->TextureID, 0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, graphics->vertexbuffer);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexdata.size(), &vertexdata[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, graphics->colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvdata.size(), &uvdata[0], GL_DYNAMIC_DRAW);
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
	//glDeleteBuffers(1, &graphics->vertexbuffer);
	//glDeleteBuffers(1, &graphics->colorbuffer);
}

void Chunk::Generate(short number) {
	//std::cout << "Generating noise for chunk " << x << " " << y << " " << z << "\n";
	for (short i = 0; i < number; i++) {
		// surface noise
		if (ypart == 0) {
			noise.SetNoiseType(FastNoise::Perlin);
			// default is 0.01
			noise.SetFrequency(0.02);
			for (int i = 0; i < chunksize; i++) {
				for (int j = 0; j < chunksize; j++) {
					heightmap[i][j] = noise.GetNoise((i + x * chunksize), (j + z * chunksize)) * 30;
				}
			}
		}
		for (int cx = 0; cx < chunksize; cx++) {
			for (int cz = 0; cz < chunksize; cz++) {
				if (((y * chunksize) + ypart) == (int)heightmap[cx][cz])
					blocks.at(cx).at(ypart).at(cz).id = 2;
				else if (((y * chunksize) + ypart) < (int)heightmap[cx][cz] - 4)
					blocks.at(cx).at(ypart).at(cz).id = 3;
				else if (((y * chunksize) + ypart) < (int)heightmap[cx][cz])
					blocks.at(cx).at(ypart).at(cz).id = 1;
				else
					blocks.at(cx).at(ypart).at(cz).id = 0;
			}
		}
		ypart++;
		if (ypart == chunksize) {
			generated = true;
		}
	}
}
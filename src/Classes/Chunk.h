#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Block.h"
#include "../Namespaces/Namespaces.h"
#include "Graphics.h"
#include "../External Stuff/FastNoise.h"

class Chunk {
public:
	Chunk(Graphics* setgraphics, int setx, int sety, int setz);
	void SetVertices();
	~Chunk();
	void Render();
	void Generate();
	void MoveX(int distance);
	void MoveY(int distance);
	void MoveZ(int distance);

	int x, y, z;
	Graphics* graphics;
	std::vector<std::vector<std::vector<Block>>> blocks;
private:
	struct blockBufferIndex {
	public:
		blockBufferIndex(int setx, int sety, int setz, size_t setvertexindex, size_t setuvindex) {
			x = setx;
			y = sety;
			z = setz;
			vertexindex = setvertexindex;
			uvindex = setuvindex;
		}

		int x, y, z, uvindex, vertexindex;
	};
	struct OtherChunks {
		// y+
		Chunk* top;
		// y-
		Chunk* bottom;
		// x-
		Chunk* left;
		// x+
		Chunk* right;
		// z-
		Chunk* back;
		// z+
		Chunk* forward;
	};
public:
	OtherChunks otherchunks;
	// Stores indices for vertex and uv buffer data for each block
	std::vector<blockBufferIndex> bufferIndices;
	std::vector<GLfloat> vertexdata;
	std::vector<GLfloat> uvdata;
};
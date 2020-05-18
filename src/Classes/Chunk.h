#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <math.h>
#include <corecrt_math_defines.h>

#include "Camera.h"
#include "Block.h"
#include "../Namespaces/Namespaces.h"
#include "Graphics.h"
#include "../External Stuff/FastNoise.h"

class Chunk {
public:
	Chunk(Graphics* setgraphics, int setx, int sety, int setz);
	~Chunk();
	void RemoveBlock(int x, int y, int z);
	bool UpdateBlock(int x, int y, int z, bool loading = false);
	void BeginLoading();
	bool KeepLoading();
	void SetVertices();
	void Render();
	void Generate();
	void MoveX(int distance);
	void MoveY(int distance);
	void MoveZ(int distance);
	bool InView(Camera* camera);

	int x, y, z;
	// These are related to loading vertices
	bool bufferloaded = false;
	bool firsttime = true;
	int cx = 0;

	// Pointer to window object
	Graphics* graphics;
	// Block data that isn't buffer data
	std::vector<std::vector<std::vector<Block>>> blocks;
	std::array<std::array<float, 16>, 16>* heightmap;
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
		OtherChunks() {
			top = bottom = left = right = back = forward = nullptr;
		}
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
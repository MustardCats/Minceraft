#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Block {
public:
	Block() {};
	Block(int setx, int sety, int setz, int id) {
		x = setx;
		y = sety;
		z = setz;
		this->id = id;
	}

	~Block() {

	}

	int x, y, z, id;
};
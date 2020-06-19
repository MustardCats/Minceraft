#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Block {
public:
	Block() {
		this->id = 0;
	};
	Block(int id) {
		this->id = id;
	}

	~Block() {

	}

	int id;
};
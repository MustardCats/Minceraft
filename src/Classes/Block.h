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
		//std::cout << "coord: " << x << " " << y << " " << z << "\n";

		this->id = id;

		x *= 2;
		y *= 2;
		z *= 2;

		top = bottom = left = right = front = back = false;
		vertices = new GLfloat[108]{
			// down front face
			x - 1.0f,y - 1.0f,z - 1.0f, // forward down left
			x - 1.0f,y - 1.0f,z + 1.0f, // forward down right
			x - 1.0f,y + 1.0f,z + 1.0f, // forward up right

			// up front face
			x - 1.0f,y - 1.0f,z - 1.0f, // forward down left
			x - 1.0f,y + 1.0f,z + 1.0f, // forward up right
			x - 1.0f,y + 1.0f,z - 1.0f, // forward up left

			// down left face
			x + 1.0f,y - 1.0f,z - 1.0f, // backward down left
			x - 1.0f,y - 1.0f,z - 1.0f, // forward down left
			x - 1.0f,y + 1.0f,z - 1.0f, // forward up left

			// up left face
			x - 1.0f,y + 1.0f,z - 1.0f, // forward up left
			x + 1.0f,y + 1.0f,z - 1.0f, // backward up left
			x + 1.0f,y - 1.0f,z - 1.0f, // backward down left

			// back left bottom face
			x + 1.0f,y - 1.0f,z - 1.0f, // backward down left
			x + 1.0f,y - 1.0f,z + 1.0f, // backward down right
			x - 1.0f,y - 1.0f,z - 1.0f, // forward down left


			// up right bottom face
			x + 1.0f,y - 1.0f,z + 1.0f, // backward down left
			x - 1.0f,y - 1.0f,z + 1.0f, // forward down right
			x - 1.0f,y - 1.0f,z - 1.0f, // forward down left

			// right
			x - 1.0f,y - 1.0f,z + 1.0f, // forward down right
			x + 1.0f,y - 1.0f,z + 1.0f, // backward down right
			x + 1.0f,y + 1.0f,z + 1.0f, // backward up right

			// right
			x - 1.0f,y + 1.0f,z + 1.0f, // forward up right
			x - 1.0f,y - 1.0f,z + 1.0f, // forward down right
			x + 1.0f,y + 1.0f,z + 1.0f, // backward up right


			// back
			x + 1.0f,y - 1.0f,z + 1.0f, // backward down right
			x + 1.0f,y - 1.0f,z - 1.0f, // backward down left
			x + 1.0f,y + 1.0f,z - 1.0f, // backward up left

			// back
			x + 1.0f,y + 1.0f,z + 1.0f, // backward up right
			x + 1.0f,y - 1.0f,z + 1.0f, // backward down right
			x + 1.0f,y + 1.0f,z - 1.0f, // backward up left

			// top
			x + 1.0f,y + 1.0f,z - 1.0f, // backward up left
			x - 1.0f,y + 1.0f,z - 1.0f, // forward up left
			x + 1.0f,y + 1.0f,z + 1.0f, // backward up right
			// top
			x - 1.0f,y + 1.0f,z - 1.0f, // forward up left
			x - 1.0f,y + 1.0f,z + 1.0f,  // forward up right
			x + 1.0f,y + 1.0f,z + 1.0f, // backward up right
		};
		uvcoords = new GLfloat[108]{
			// forward
			0.0f, 0.5f,
			0.25f, 0.5f,
			0.25f, 0.25f,

			0.0f, 0.5f,
			0.25f, 0.25f,
			0.0f, 0.25f,
			//left
			0.0f, 0.5f,
			0.25f, 0.5f,
			0.25f, 0.25f,

			0.25f, 0.25f,
			0.0f, 0.25f,
			0.0f, 0.5f,
			// bottom
			0.0f, 0.25f,
			0.25f, 0.25f,
			0.0f, 0.5f,

			0.25f, 0.25f,
			0.25f, 0.5f,
			0.0f, 0.5f,

			// right
			0.0f, 0.5f,
			0.25f, 0.5f,
			0.25f, 0.25f,

			0.0f, 0.25f,
			0.0f, 0.5f,
			0.25f, 0.25f,
			// back
			0.0f, 0.5f,
			0.25f, 0.5f,
			0.25f, 0.25f,

			0.0f, 0.25f,
			0.0f, 0.5f,
			0.25f, 0.25f,
			// top
			0.0f, 0.25f,
			0.0f, 0.5f,
			0.25f, 0.25f,

			0.0f, 0.5f,
			0.25f, 0.5f,
			0.25f, 0.25f,
		};

		x /= 2;
		y /= 2;
		z /= 2;
	}

	~Block() {

	}

	bool IsActive() {
		return top | bottom | left | right | front | back;
	}
	
	bool top, bottom, left, right, front, back;
	int x, y, z, id;

	GLfloat* vertices;
	GLfloat* uvcoords;
};
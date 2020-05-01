#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glut.h>
#include <SFML/Graphics.hpp>
#include <SOIL/SOIL.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <fstream>

#include "../Namespaces/Namespaces.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class TextureAtlas {
public:
	TextureAtlas(const std::string path, unsigned int setresolution) {
		resolution = setresolution;
		LoadPNG(path);
	}

	void loadFromImage(const sf::Image& i);

	void LoadPNG(const std::string& file);

	misc::uvcoords GetUvCoords(int x, int y);
	GLuint LoadDDS(const char* path);
	GLuint LoadBMP(const char* path);

	unsigned char* header;

	int width;
	int height;
	unsigned int mipMapCount;

	unsigned int blockSize;
	unsigned int format;

	unsigned int resolution;
	GLuint texture;
	GLuint TextureID;
};
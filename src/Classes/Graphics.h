#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glut.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Camera.h"
#include "TextureAtlas.h"

class Graphics {
public:
	Graphics();
	~Graphics();

	bool Update(float deltatime);
	void DrawRect(std::vector<GLfloat>* vertexdata, std::vector<GLfloat>* uvdata);
	bool SetUp();
	void DrawStaticRect(float x, float y, float w, float h);

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

	GLuint loadBMP_custom(const char* imagepath);

	GLuint loadDDS(const char* imagepath);

	GLFWwindow* window;
	GLuint colorProgramID;
	GLuint staticProgramId;
	Camera* camera;
	glm::mat4 MVP;
	GLuint matrixID;

	GLuint VertexArrayID;

	TextureAtlas* sheet1;


	int SCREENWIDTH, SCREENHEIGHT;
};
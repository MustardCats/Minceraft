#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Namespaces/Namespaces.h"

class Camera {
public:
	Camera(GLFWwindow* setwindow, GLuint& programID, int x, int y, int z);
	~Camera();

	bool Update(float deltatime);

	void MoveSideways(float distance);

	void MoveForward(float distance);

	void MoveVertically(float distance);

	GLuint matrixID;
	glm::vec3 position;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	float horizontalAngle = 208;
	float verticalAngle = 1149;
	float mouseSpeed = 0.005f;
	float initialFoV = 45.0f;
	float speed = 5.0f;
	bool firstTime = true;

	int SCREENWIDTH = 1024;
	int SCREENHEIGHT = 768;

	GLFWwindow* window; 
	misc::CanMove canmove;
};
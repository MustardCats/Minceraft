#include "../Classes/Camera.h"

Camera::Camera(GLFWwindow* setwindow, GLuint& programID,  int x, int y, int z) {
	window = setwindow;
	position = glm::vec3(x, y, z);
	horizontalAngle = 1.55;
	verticalAngle = 0;
	// make cursor invisible
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Camera::Update(float deltatime) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		std::cout << "Position: " << position[0] << " " << position[1] << " " << position[2] << "\n";
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	static bool iscentered = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		iscentered = false;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		iscentered = true;
	if (iscentered) {
		// Reset mouse position for next frame
		glfwSetCursorPos(window, SCREENWIDTH / 2, SCREENHEIGHT / 2);
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(SCREENWIDTH / 2 - xpos);
		verticalAngle += mouseSpeed * float(SCREENHEIGHT / 2 - ypos);
		if (verticalAngle > 1.628)
			verticalAngle = 1.628;
		if (verticalAngle < -1.628)
			verticalAngle = -1.628;
	}

	if (firstTime) {
		horizontalAngle = 1.55;
		verticalAngle = 0;
		firstTime = false;
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.1415f / 2.0f),
		0,
		cos(horizontalAngle - 3.1415f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)((float)SCREENWIDTH / (float)SCREENHEIGHT), 0.1f, 150.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	return true;
}
// I made this myself!!!
void Camera::MoveSideways(float distance) {
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.1415f / 2.0f),
		0,
		cos(horizontalAngle - 3.1415f / 2.0f)
	);
	// If the camera can't move a certain direction, multiply the distance with
	// the appropriate vector result and make it zero if needed
	if ((right[0] * distance > 0 && !canmove.px) || (right[0] * distance < 0 && !canmove.nx))
		right[0] = 0;
	if ((right[2] * distance > 0 && !canmove.pz) || (right[2] * distance < 0 && !canmove.nz))
		right[2] = 0;

	position += right * distance;
}

void Camera::MoveForward(float distance) {
	glm::vec3 forward = glm::vec3(
		-cos(horizontalAngle + 3.1415f / 2.0f),
		0,
		-sin(horizontalAngle - 3.1415f / 2.0f)
		//cos(verticalAngle) * sin(horizontalAngle),
		//sin(verticalAngle),
		//cos(verticalAngle) * cos(horizontalAngle)
	);

	if ((forward[0] * distance > 0 && !canmove.px) || (forward[0] * distance < 0 && !canmove.nx))
		forward[0] = 0;
	if ((forward[2] * distance > 0 && !canmove.pz) || (forward[2] * distance < 0 && !canmove.nz))
		forward[2] = 0;

	position += forward * distance;
}

void Camera::MoveVertically(float distance) {
	if (!((!canmove.ny && distance < 0) || (!canmove.py && distance > 0)))
		position[1] += distance;
}
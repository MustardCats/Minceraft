#include "../Classes/App.h"

App::App() {

}

App::~App() {

}

bool App::SetUp() {
	graphics = new Graphics();
	graphics->SetUp();
	stage = new Stage(graphics, 1, 1, 1);
	player = new Player(graphics);
	graphics->camera = player->camera;
	return true;
}

bool App::Start() {
	bool success = true;
	float deltatime = 0;
	auto start = std::chrono::high_resolution_clock::now();
	while (DoFrame(deltatime) && success && !glfwWindowShouldClose(graphics->window)) {
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		static int count = 0;
		static float totaltime = 0;
		deltatime = duration.count() / 1000.0;
		totaltime += deltatime;
		count++;
		if (totaltime >= 1) {
			std::string title = "FPS: " + std::to_string(count);
			glfwSetWindowTitle(graphics->window, title.c_str());
			//std::cout << deltatime << "\n";
			count = 0;
			totaltime = 0;
		}
		start = std::chrono::high_resolution_clock::now();
	}
	if (success) {
		End();
	}

	return success;
}

bool App::DoFrame(float deltatime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	player->Update(deltatime, stage);
	graphics->Update(deltatime);
	stage->Update(player->chunkpos, player->chunkblockpos, deltatime);
	

	graphics->DrawStaticRect(0, 0, 0.3, 0.3);

	for (int cx = 0; cx < stage->width; cx++) {
		for (int cy = 0; cy < stage->height; cy++) {
			for (int cz = 0; cz < stage->width; cz++) {
				//graphics->DrawRect(&stage->chunks.at((cx * stage->height * stage->width) + (cy * stage->height) + cz)->vertexdata, &stage->chunks.at((cx * stage->height * stage->width) + (cy * stage->height) + cz)->uvdata);
			}
		}
	} 

	for (int i = 0; i < stage->chunks.size(); i++) {
		stage->chunks.at(i)->Render();
	}

	static bool dostuff = true;
	if (glfwGetKey(graphics->window, GLFW_KEY_C) == GLFW_PRESS && dostuff) {
		std::cout << "Moving Chunk!\n";
		stage->chunks.at(0)->MoveZ(5);
		stage->chunks.at(0)->Generate();
		stage->chunks.at(0)->SetVertices();
		dostuff = false;
	}
	glfwSwapBuffers(graphics->window);
	glfwPollEvents();
	//std::cout << graphics->camera->position[0] << std::endl;
	return true;
}

void App::End() {

}

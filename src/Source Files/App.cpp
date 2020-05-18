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
	double deltatime = 0;
	auto start = std::chrono::high_resolution_clock::now();
	while (DoFrame(deltatime) && success && !glfwWindowShouldClose(graphics->window)) {
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		static int count = 0;
		static double totaltime = 0;
		deltatime = duration.count() / 1000000.0;
		totaltime += deltatime;
		count++;
		if (totaltime >= 1.0) {
			std::string title = "FPS: " + std::to_string(count);
			glfwSetWindowTitle(graphics->window, title.c_str());
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
	
	if (stage->FindChunk(player->chunkpos.x, player->chunkpos.y, player->chunkpos.z) != nullptr || player->debug)
		player->Update(deltatime, stage);
	graphics->Update(deltatime);
	//stage->Update(player->chunkpos, player->chunkblockpos, deltatime);
	graphics->DrawStaticRect(0, 0, 0.3, 0.3);
	
	//stage->chunks.at(0)->InFrontPlayer(player->camera->horizontalAngle, player->camera->verticalAngle, player->camera->position);

	for (int i = 0; i < stage->chunks.size(); i++) {
		if (stage->chunks.at(i)->vertexdata.size() > 0) {
			stage->chunks.at(i)->Render();
		}
	}
	//stage->Render();
	if (stage->chunks.at(0)->InView(player->camera))
		std::cout << "Chunk in view!\n";
	else {
		std::cout << "Chunk not in view!\n";
	}

	glfwSwapBuffers(graphics->window);
	glfwPollEvents();
	//std::cout << graphics->camera->position[0] << std::endl;
	
	return true;
}

void App::End() {

}

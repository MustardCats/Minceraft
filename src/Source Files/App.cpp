#include "../Classes/App.h"

App::App() {

}

App::~App() {
	delete stage;
	delete player;
	delete graphics;
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
	static short framecount = 1;
	while (success && !glfwWindowShouldClose(graphics->window)) {
		success = DoFrame(deltatime, framecount);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		static int count = 0;
		static double totaltime = 0;
		deltatime = duration.count() / 1000000.0;
		totaltime += deltatime;
		count++;
		framecount++;
		if (totaltime >= 1.0) {
			std::string title = "FPS: " + std::to_string(count);
			glfwSetWindowTitle(graphics->window, title.c_str());
			count = 0;
			totaltime = 0;
		}
		if (framecount >= 360) {
			framecount = 1;
		}
		start = std::chrono::high_resolution_clock::now();
	}
	success = End();

	return success;
}

bool App::DoFrame(float deltatime, short framecount) {
	if (stage->FindChunk(player->chunkpos.x, player->chunkpos.y, player->chunkpos.z) != nullptr || player->debug)
		player->Update(deltatime, stage);
	graphics->Update(deltatime);
	//stage->Update(player->chunkpos, player->chunkblockpos, deltatime);
	if (framecount % 3 == 0) {
		//std::thread cat(&Stage::MakeChunks, stage, player->chunkpos, player->chunkblockpos);
		//cat.join();
		stage->MakeChunks(player->chunkpos, player->chunkblockpos);
	}
	//std::thread thread(&Stage::MakeChunks, stage, player->chunkpos, player->chunkblockpos);
	graphics->DrawStaticRect(0, 0, 0.1, 0.1);
	//stage->chunks.at(0)->InFrontPlayer(player->camera->horizontalAngle, player->camera->verticalAngle, player->camera->position);
	if (player->chunk != nullptr || true) {
		for (int i = 0; i < stage->chunks.size(); i++) {
			if (stage->chunks.at(i)->vertexdata.size() > 0) {
				if (stage->chunks.at(i)->InView(player->camera) || true) {
					stage->chunks.at(i)->Render();
				}
			}
		}
	}
	
	glfwSwapBuffers(graphics->window);
	glfwPollEvents();
	//std::cout << graphics->camera->position[0] << std::endl;
	
	return true;
}

bool App::End() {
	return true;
}

#include "../Classes/Player.h"

Player::Player(Graphics* setgraphics) {
	graphics = setgraphics;
	camera = new Camera(graphics->window, graphics->colorProgramID, -3, 0, 0);
	
	debug = false;
	isJumping = false;
	isRunning = false;

	jumpUpgrade = 1;
	hVelocity = 1.5;
}

Player::~Player() {

}

bool Player::Update(float deltatime, Stage* stage) {
	MoveHorizontal(deltatime);
	MoveVertical(deltatime);
	// Update position
	x = camera->position[0];
	y = camera->position[1];
	z = camera->position[2];

	if (x > 0)
		chunkpos.x = floor(x / misc::chunkSize.x);
	else
		chunkpos.x = ceil(x / misc::chunkSize.x) - 1;
	if (y > 0)
		chunkpos.y = floor(y / misc::chunkSize.y);
	else
		chunkpos.y = ceil(y / misc::chunkSize.y) - 1;
	if (z > 0)
		chunkpos.z = floor(z / misc::chunkSize.z);
	else
		chunkpos.z = ceil(z / misc::chunkSize.z) - 1;

	chunkblockpos.x = abs((int)x % misc::chunkSize.x);
	chunkblockpos.y = abs((int)y % misc::chunkSize.y);
	chunkblockpos.z = abs((int)z % misc::chunkSize.z);
	chunk = stage->FindChunk(chunkpos.x, chunkpos.y, chunkpos.z);

	// toggle debug mode
	if (true) {
		static bool pressed;
		if (glfwGetKey(graphics->window, GLFW_KEY_F) == GLFW_PRESS && !pressed) {
			if (debug)
				debug = false;
			else
				debug = true;
			pressed = true;
		}
		else {
			pressed = false;
		}
	}

	if (chunk != nullptr && !debug) {
		//std::cout << "Player's chunk is at " << chunk->x << " " << chunk->y << " " << chunk->z << " " << x << " " << y << " " << z << "\n";
		DetectCollision(stage);
	}
	else {
		camera->canmove.nx = camera->canmove.px = camera->canmove.ny = camera->canmove.py = camera->canmove.nz = camera->canmove.pz = true;
	}
	camera->Update(deltatime);

	if (true) {
		static bool press = true;
		if (glfwGetMouseButton(graphics->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && press) {
			Ray ray(stage, camera->position, camera->horizontalAngle, camera->verticalAngle, 10);
			press = false;
		}
		else if (!glfwGetMouseButton(graphics->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			press = true;
	}

	return true;
}

void Player::DetectCollision(Stage* stage) {
	camera->canmove.nx = camera->canmove.px = camera->canmove.ny = camera->canmove.py = camera->canmove.nz = camera->canmove.pz = true;
	Block block;
	block.id = 0;
	// bottom
	if (stage->SearchChunks(block, round(x), round(y - 1.5), round(z), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.ny = false;
	}
	block.id = 0;
	if (stage->SearchChunks(block, round(x), round(y + 0.5), round(z), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.py = false;
	}
	block.id = 0;
	if (stage->SearchChunks(block, floor(x + 0.9), round(y - 0.5), round(z), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.px = false;
	}
	if (stage->SearchChunks(block, ceil(x - 0.9), round(y - 0.5), round(z), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.nx = false;
	}
	if (stage->SearchChunks(block, round(x), round(y - 0.5), floor(z + 0.9), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.pz = false;
	}
	if (stage->SearchChunks(block, round(x), round(y - 0.5), ceil(z - 0.9), nullptr) != 0) {
		if (block.id != 0)
			camera->canmove.nz = false;
	}
	/*
	// negative x
	if (SearchChunks(ceil(x - 1), floor(y - 0.5), round(z), block) != 0
		|| SearchChunks(ceil(x - 1), round(y + 0.5), round(z), block) != 0) {
		if (block.id != 0)
			camera->canmove.nx = false;
	}
	block.id = 0;
	// positive x
	if (SearchChunks(floor(x + 1), floor(y - 0.5), round(z), block) != 0) {
		if (block.id != 0)
			camera->canmove.px = false;
	}
	block.id = 0;
	// positive z
	if (SearchChunks(round(x), floor(y - 0.5), floor(z + 1), block) != 0) {
		if (block.id != 0)
			camera->canmove.pz = false;
	}
	block.id = 0;
	// negative z
	if (SearchChunks(round(x), floor(y - 0.5), ceil(z - 1), block) != 0) {
		if (block.id != 0)
			camera->canmove.nz = false;
	}*/
}

void Player::MoveHorizontal(float deltatime) {
	static float acceleration = 8.0;
	bool isWalking = true;
	if (!isJumping &&
		!glfwGetKey(graphics->window, GLFW_KEY_W) == GLFW_PRESS &&
		!glfwGetKey(graphics->window, GLFW_KEY_S) == GLFW_PRESS &&
		!glfwGetKey(graphics->window, GLFW_KEY_A) == GLFW_PRESS &&
		!glfwGetKey(graphics->window, GLFW_KEY_D) == GLFW_PRESS) {
		hVelocity = 1.5;
		isWalking = false;
	}
	// To run or not run, you need to be on the ground and hold shift
	if (glfwGetKey(graphics->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !isJumping && !isRunning) {
		isRunning = true;
		// Needs this bool or changing to run will halt movement
		if (!isWalking)
			hVelocity = 3;
		acceleration = 4.0;
	}
	if (!glfwGetKey(graphics->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !isJumping && isRunning) {
		isRunning = false;
		// Set to walking maximum so stopping running doesn't halt movement
		hVelocity = 6.0;
		acceleration = 8.0;
	}
	// Update velocity
	if ((isRunning && hVelocity < 8.0) || (!isRunning && hVelocity < 6)) {
		hVelocity += (acceleration * deltatime);
	}
	// The !isJumping makes it so fast speed when run-jumping isn't cancelled
	if (isRunning && hVelocity > 8.0 && !isJumping)
		hVelocity = 8.0;
	if (!isRunning && hVelocity > 6.0)
		hVelocity = 6.0;
	
	//std::cout << "Velocity: " << hVelocity << "\n";
	// Actually move
	if (glfwGetKey(graphics->window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->MoveForward(deltatime * hVelocity);
	}
	// Move backward
	if (glfwGetKey(graphics->window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->MoveForward(-deltatime * hVelocity);
	}
	if (glfwGetKey(graphics->window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->MoveSideways(deltatime * 3.0);
	}
	// Strafe left
	if (glfwGetKey(graphics->window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->MoveSideways(-deltatime * 3.0);
	}
}

void Player::MoveVertical(float deltatime) {
	if (!debug) {
		static float acceleration;
		// If on ground
		if (!camera->canmove.ny) {
			isJumping = false;
			vVelocity = 0.0;
			// This and the other isRunning assignment are a double-check for
			// allowing the change between jump and long jump without having
			// to stop jumping
			if (!glfwGetKey(graphics->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				isRunning = false;
		}
		else if (glfwGetKey(graphics->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			isRunning = true;
			// Long Jump!
			if (jumpUpgrade > 0 && isJumping) {
				hVelocity = 12.0;
			}
		}
		/*
		For some reason not including a random check for space here
		results in the following check for space acting weird, causing
		the player to bounce after jumping. Try removing it to see!
		*/
		if (glfwGetKey(graphics->window, GLFW_KEY_SPACE) == GLFW_PRESS) {

		}
		// Start jumping
		if (!isJumping && !camera->canmove.ny) {
			if (glfwGetKey(graphics->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				// If running
				if (isRunning && jumpUpgrade > 0) {
					vVelocity = 6.0;
					acceleration = 16.0;
				}
				// If walking
				else {
					vVelocity = 7.0;
					acceleration = 15.0;
				}
				isJumping = true;

			}
		}
		// Update velocity
		if (abs(vVelocity) < 20.0 || true)
			vVelocity -= (acceleration * deltatime);

		camera->MoveVertically(vVelocity * deltatime);
	}
	else {
		if (glfwGetKey(graphics->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera->MoveVertically(10 * deltatime);
		}
		if (glfwGetKey(graphics->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			camera->MoveVertically(-10 * deltatime);
		}
	}
}
// tx means target x (MAKE FASTER!!!)
int Player::SearchChunks(Stage* stage, Block& block, int tx, int ty, int tz) {
	misc::tcoord localchunk;
	
	localchunk.x = ceil(tx / misc::chunkSize.x);
	localchunk.y = ceil(ty / misc::chunkSize.y);
	localchunk.z = ceil(tz / misc::chunkSize.z);

	if (tx < 0)
		localchunk.x--;
	if (ty < 0)
		localchunk.y--;
	if (tz < 0)
		localchunk.z--;

	misc::tcoord localblock;
	for (int i = 0; i < stage->chunks.size(); i++) {
		if (stage->chunks.at(i)->x == localchunk.x
			|| stage->chunks.at(i)->y == localchunk.y
			|| stage->chunks.at(i)->z == localchunk.z) {
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (stage->chunks.at(i)->blocks.at(cx).at(0).at(0).x == tx) {
					localblock.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (stage->chunks.at(i)->blocks.at(0).at(cy).at(0).y == ty) {
							localblock.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).z == tz
									&& stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).x == tx
									&& stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).y == ty) {
									std::cout << "Found block " << stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).id <<
										" at " << stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).x << " "
										<< stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).y << " "
										<< stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz).z << "\n";
									block = stage->chunks.at(i)->blocks.at(cx).at(cy).at(cz);
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}




/*	//-16 -15 -9
	// Search Main Chunk
	misc::tcoord localchunk;
	if (tx > 0)
		localchunk.x = (int)tx / misc::chunkSize.x;
	else
		localchunk.x = ((int)tx / misc::chunkSize.x) - 1;
	if (ty > 0)
		localchunk.y = (int)ty / misc::chunkSize.y;
	else
		localchunk.y = ((int)ty / misc::chunkSize.y) - 1;
	if (tz > 0)
		localchunk.z = (int)tz / misc::chunkSize.z;
	else
		localchunk.z = ((int)tz / misc::chunkSize.z) - 1;

	misc::tcoord localpos;
	// I used to check whether chunk is relavent and skip if not
	// But it caused some weird collision issues, so no
	for (int cx = 0; cx < misc::chunkSize.x; cx++) {
		if (chunk->blocks.at(cx).at(0).at(0).x == tx) {
			localpos.x = cx;
			for (int cy = 0; cy < misc::chunkSize.y; cy++) {
				if (chunk->blocks.at(localpos.x).at(cy).at(0).y == ty) {
					localpos.y = cy;
					for (int cz = 0; cz < misc::chunkSize.z; cz++) {
						if (chunk->blocks.at(localpos.x).at(localpos.y).at(cz).z == tz) {
							std::cout << "Main Chunk: \n";
							std::cout << chunk->blocks.at(cx).at(cy).at(cz).id << " "
								<< chunk->blocks.at(cx).at(cy).at(cz).x << " "
								<< chunk->blocks.at(cx).at(cy).at(cz).y << " "
								<< chunk->blocks.at(cx).at(cy).at(cz).z << "\n";
							block = chunk->blocks.at(localpos.x).at(localpos.y).at(cz);
							return 1;
						}
					}
				}
			}
		}
	}
	// bottom chunk
	if (chunk->otherchunks.bottom != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.bottom->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.bottom->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.bottom->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "Bottom Chunk: \n";
								std::cout << chunk->otherchunks.bottom->blocks.at(localpos.x).at(localpos.y).at(cz).id << " "
									<< chunk->otherchunks.bottom->blocks.at(localpos.x).at(localpos.y).at(cz).x << " "
									<< chunk->otherchunks.bottom->blocks.at(localpos.x).at(localpos.y).at(cz).y << " "
									<< chunk->otherchunks.bottom->blocks.at(localpos.x).at(localpos.y).at(cz).z << "\n";
								block = chunk->otherchunks.bottom->blocks.at(localpos.x).at(localpos.y).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	else {
		std::cout << "no bottom chunk!\n";
	}
	// left chunk
	if (chunk->otherchunks.left != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.left->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.left->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.left->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "left " << chunk->otherchunks.left->blocks.at(cx).at(cy).at(cz).id << std::endl;
								block = chunk->otherchunks.left->blocks.at(cx).at(cy).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	// right chunk
	if (chunk->otherchunks.right != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.right->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.right->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.right->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "right " << chunk->otherchunks.right->blocks.at(cx).at(cy).at(cz).id << std::endl;
								block = chunk->otherchunks.right->blocks.at(localpos.x).at(localpos.y).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	// top chunk
	if (chunk->otherchunks.top != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.top->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.top->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.top->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "top " << chunk->otherchunks.top->blocks.at(cx).at(cy).at(cz).id << std::endl;
								block = chunk->otherchunks.top->blocks.at(localpos.x).at(localpos.y).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	// back chunk
	if (chunk->otherchunks.back != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.back->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.back->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.back->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "back " << chunk->otherchunks.back->blocks.at(cx).at(cy).at(cz).id << std::endl;
								block = chunk->otherchunks.back->blocks.at(localpos.x).at(localpos.y).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	// front chunk
	if (chunk->otherchunks.forward != nullptr) {
		misc::tcoord localpos;
		for (int cx = 0; cx < misc::chunkSize.x; cx++) {
			if (chunk->otherchunks.forward->blocks.at(cx).at(0).at(0).x == tx) {
				localpos.x = cx;
				for (int cy = 0; cy < misc::chunkSize.y; cy++) {
					if (chunk->otherchunks.forward->blocks.at(cx).at(cy).at(0).y == ty) {
						localpos.y = cy;
						for (int cz = 0; cz < misc::chunkSize.z; cz++) {
							if (chunk->otherchunks.forward->blocks.at(cx).at(cy).at(cz).z == tz) {
								std::cout << "forward " << chunk->otherchunks.forward->blocks.at(cx).at(cy).at(cz).id << std::endl;
								block = chunk->otherchunks.forward->blocks.at(localpos.x).at(localpos.y).at(cz);
								return 1;
							}
						}
					}
				}
			}
		}
	}

	// CORNERS
	if (chunk->otherchunks.back != nullptr) {
		if (chunk->otherchunks.back->otherchunks.left != nullptr) {
			misc::tcoord localpos;
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (chunk->otherchunks.back->otherchunks.left->blocks.at(cx).at(0).at(0).x == tx) {
					localpos.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (chunk->otherchunks.back->otherchunks.left->blocks.at(cx).at(cy).at(0).y == ty) {
							localpos.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (chunk->otherchunks.back->otherchunks.left->blocks.at(cx).at(cy).at(cz).z == tz) {
									block = chunk->otherchunks.back->otherchunks.left->blocks.at(localpos.x).at(localpos.y).at(cz);
									return 1;
								}
							}
						}
					}
				}
			}
		}
		if (chunk->otherchunks.back->otherchunks.right != nullptr) {
			misc::tcoord localpos;
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (chunk->otherchunks.back->otherchunks.right->blocks.at(cx).at(0).at(0).x == tx) {
					localpos.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (chunk->otherchunks.back->otherchunks.right->blocks.at(cx).at(cy).at(0).y == ty) {
							localpos.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (chunk->otherchunks.back->otherchunks.right->blocks.at(cx).at(cy).at(cz).z == tz) {
									block = chunk->otherchunks.back->otherchunks.right->blocks.at(localpos.x).at(localpos.y).at(cz);
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	if (chunk->otherchunks.forward != nullptr) {
		if (chunk->otherchunks.forward->otherchunks.left != nullptr) {
			misc::tcoord localpos;
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (chunk->otherchunks.forward->otherchunks.left->blocks.at(cx).at(0).at(0).x == tx) {
					localpos.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (chunk->otherchunks.forward->otherchunks.left->blocks.at(cx).at(cy).at(0).y == ty) {
							localpos.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (chunk->otherchunks.forward->otherchunks.left->blocks.at(cx).at(cy).at(cz).z == tz) {
									block = chunk->otherchunks.forward->otherchunks.left->blocks.at(localpos.x).at(localpos.y).at(cz);
									return 1;
								}
							}
						}
					}
				}
			}
		}
		if (chunk->otherchunks.forward->otherchunks.right != nullptr) {
			misc::tcoord localpos;
			for (int cx = 0; cx < misc::chunkSize.x; cx++) {
				if (chunk->otherchunks.forward->otherchunks.right->blocks.at(cx).at(0).at(0).x == tx) {
					localpos.x = cx;
					for (int cy = 0; cy < misc::chunkSize.y; cy++) {
						if (chunk->otherchunks.forward->otherchunks.right->blocks.at(cx).at(cy).at(0).y == ty) {
							localpos.y = cy;
							for (int cz = 0; cz < misc::chunkSize.z; cz++) {
								if (chunk->otherchunks.forward->otherchunks.right->blocks.at(cx).at(cy).at(cz).z == tz) {
									block = chunk->otherchunks.forward->otherchunks.right->blocks.at(localpos.x).at(localpos.y).at(cz);
									return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	std::cout << "Couldn't find block at " << tx << " " << ty << " " << tz << "\n";*/

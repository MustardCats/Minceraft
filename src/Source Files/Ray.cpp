#include "../Classes/Ray.h"

Ray::Ray(Stage* setstage, glm::vec3 setposition, float& sethorizontal, float& setvertical, float setmaxlength) {
	stage = setstage;
	position = setposition;
	vertical = setvertical;
	horizontal = sethorizontal;
	maxlength = setmaxlength;

	while (Step() && length < maxlength);

	this->~Ray();
}

Ray::~Ray() {

}

bool Ray::Step() {
	Block block;
	misc::chunkindex index;

	int x, y, z;

	if (round(position[0] < position[0]))
		x = ceil(position[0]);
	else
		x = round(position[0]);
	if (round(position[1] < position[1]))
		y = ceil(position[1]);
	else
		y = round(position[1]);
	if (round(position[2] < position[2]))
		z = ceil(position[2]);
	else
		z = round(position[2]);

	if (stage->SearchChunks(block, x, y, z, &index) == 1) {
		if (block.id != 0) {
			std::cout << "Block Broken was at " << x << " " << y << " " << z <<
				" and the indices are " << index.bx << " " << index.by << " " << index.bz << "\n";
			// Reset block data
			stage->chunks.at(index.c)->blocks.at(index.bx).at(index.by).at(index.bz).id = 0;
			// Delete buffer data
			int bufferindex;
			int uvindex;
			// Erases the block
			stage->chunks.at(index.c)->UpdateBlock(index.bx, index.by, index.bz);
			// Update surrounding blocks
			// x
			if (index.bx == 0 && stage->chunks.at(index.c)->otherchunks.left != nullptr) {
				stage->chunks.at(index.c)->otherchunks.left->UpdateBlock((chunksize - 1), index.by, index.bz);
			}
			else if (index.bx == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.right != nullptr) {
				stage->chunks.at(index.c)->otherchunks.right->UpdateBlock(0, index.by, index.bz);
			}
			if (index.bx > 0) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx - 1, index.by, index.bz);
			}
			if (index.bx < (chunksize - 1)) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx + 1, index.by, index.bz);
			}
			// y
			if (index.by == 0 && stage->chunks.at(index.c)->otherchunks.bottom != nullptr) {
				std::cout << "y" << "\n";
				stage->chunks.at(index.c)->otherchunks.bottom->UpdateBlock(index.bx, (chunksize - 1), index.bz);
			}
			else if (index.by == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.top != nullptr) {
				stage->chunks.at(index.c)->otherchunks.top->UpdateBlock(index.bx, 0, index.bz);
			}
			if (index.by > 0) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx, index.by - 1, index.bz);
			}
			if (index.by < (chunksize - 1)) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx, index.by + 1, index.bz);
			}
			// z
			if (index.bz == 0 && stage->chunks.at(index.c)->otherchunks.back != nullptr) {
				stage->chunks.at(index.c)->otherchunks.back->UpdateBlock(index.bx, index.by, (chunksize - 1));
			}
			else if (index.bz == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.forward != nullptr) {
				stage->chunks.at(index.c)->otherchunks.forward->UpdateBlock(index.bx, index.by, 0);
			}
			if (index.bz > 0) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx, index.by, index.bz - 1);
			}
			if (index.bz < (chunksize - 1)) {
				stage->chunks.at(index.c)->UpdateBlock(index.bx, index.by, index.bz + 1);
			}
			/*
			if (x >= 0) {
				if (x % 16 == 0 && stage->chunks.at(index.c)->otherchunks.left != nullptr) {
					stage->chunks.at(index.c)->otherchunks.left->SetVertices();
				}
				else if (x % 16 == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.right != nullptr) {
					stage->chunks.at(index.c)->otherchunks.right->SetVertices();
				}
			}
			else {
				if (abs((x + 1) % 16) == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.left != nullptr) {
					stage->chunks.at(index.c)->otherchunks.left->SetVertices();
				}
				if (abs((x + 1) % 16) == 0 && stage->chunks.at(index.c)->otherchunks.right != nullptr) {
					stage->chunks.at(index.c)->otherchunks.right->SetVertices();
				}
			}
			// y
			if (y >= 0) {
				if (y % 16 == 0 && stage->chunks.at(index.c)->otherchunks.bottom != nullptr) {
					stage->chunks.at(index.c)->otherchunks.bottom->SetVertices();
				}
				else if (y % 16 == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.top != nullptr) {
					stage->chunks.at(index.c)->otherchunks.top->SetVertices();
				}
			}
			else {
				if (abs((y + 1) % 16) == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.bottom != nullptr) {
					stage->chunks.at(index.c)->otherchunks.bottom->SetVertices();
				}
				if (abs((y + 1) % 16) == 0 && stage->chunks.at(index.c)->otherchunks.top != nullptr) {
					stage->chunks.at(index.c)->otherchunks.top->SetVertices();
				}
			}
			// z
			if (z >= 0) {
				if (z % 16 == 0 && stage->chunks.at(index.c)->otherchunks.back != nullptr) {
					stage->chunks.at(index.c)->otherchunks.back->SetVertices();
				}
				else if (z % 16 == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.forward != nullptr) {
					stage->chunks.at(index.c)->otherchunks.forward->SetVertices();
				}
			}
			else {
				if (abs((z + 1) % 16) == (chunksize - 1) && stage->chunks.at(index.c)->otherchunks.back != nullptr) {
					stage->chunks.at(index.c)->otherchunks.back->SetVertices();
				}
				if (abs((z + 1) % 16) == 0 && stage->chunks.at(index.c)->otherchunks.forward != nullptr) {
					stage->chunks.at(index.c)->otherchunks.forward->SetVertices();
				}
			}
			*/
			/*
			for (int i = 0; i < stage->chunks.at(index.c)->bufferIndices.size(); i++) {
				if (stage->chunks.at(index.c)->bufferIndices.at(i).x == index.bx &&
					stage->chunks.at(index.c)->bufferIndices.at(i).y == index.by &&
					stage->chunks.at(index.c)->bufferIndices.at(i).z == index.bz) {
					bufferindex = stage->chunks.at(index.c)->bufferIndices.at(i).vertexindex;
					uvindex = stage->chunks.at(index.c)->bufferIndices.at(i).uvindex;
					stage->chunks.at(index.c)->vertexdata.erase(stage->chunks.at(index.c)->vertexdata.begin() + bufferindex - 0, stage->chunks.at(index.c)->vertexdata.begin() + bufferindex + 18);
					stage->chunks.at(index.c)->uvdata.erase(stage->chunks.at(index.c)->uvdata.begin() + uvindex - 0, stage->chunks.at(index.c)->uvdata.begin() + uvindex + 12);
					for (int j = i; j < stage->chunks.at(index.c)->bufferIndices.size(); j++) {
						stage->chunks.at(index.c)->bufferIndices.at(j).uvindex -= 12;
						stage->chunks.at(index.c)->bufferIndices.at(j).vertexindex -= 18;
					}
					//break;
				}
			}*/
			return false;
		}
	}
	glm::vec3 direction = glm::vec3(
		cos(vertical) * sin(horizontal),
		sin(vertical),
		cos(vertical) * cos(horizontal)
	);

	position += direction * 0.1f;
	length += 0.1f;

	return true;
}

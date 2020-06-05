#include "../Classes/App.h"
#include "../External Stuff/FastNoise.h"

int main(int* argcp, char** argv) {
	App* app = new App();
	if (!app->SetUp()) {
		return -2;
	}
	else if (!app->Start()) {
		return -1;
	}
	else
		return 0;
}
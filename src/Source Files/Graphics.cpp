#include "../Classes/Graphics.h"

Graphics::Graphics() {
	SCREENWIDTH = 1024;
	SCREENHEIGHT = 768;
	window = nullptr;
	colorProgramID = 0;
}

Graphics::~Graphics() {
	glfwDestroyWindow(window);
}

bool Graphics::Update(float deltatime) {
	// Poll events

	int width, height;
	// Update window size
	glfwGetWindowSize(window, &width, &height);
	if (width != SCREENWIDTH || height != SCREENHEIGHT) {
		SCREENWIDTH = width;
		SCREENHEIGHT = height;
		glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);
		camera->SCREENWIDTH = SCREENWIDTH;
		camera->SCREENHEIGHT = SCREENHEIGHT;
	}
	// Update camera and matrix
	camera->Update(deltatime);
	MVP = camera->ProjectionMatrix * camera->ViewMatrix;
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

void Graphics::DrawRect(std::vector<GLfloat>* vertexdata, std::vector<GLfloat>* uvdata) {
	glUseProgram(colorProgramID);

	GLuint vertexbuffer;
	GLuint colorbuffer;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sheet1->texture);
	glUniform1i(sheet1->TextureID, 0);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexdata->size(), vertexdata->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvdata->size(), uvdata->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, vertexdata->size());

	//glDeleteProgram(colorProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//glDeleteVertexArrays(1, &VertexArrayID);
}

bool Graphics::SetUp() {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Tutorial 01", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// light blue background
	glClearColor(0.0f, 0.5f, 0.7f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MIPMAP);
	//glEnable(GL_SCISSOR_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	colorProgramID = LoadShaders("shaders/texturevertex.shader", "shaders/texturefragment.shader");
	staticProgramId = LoadShaders("shaders/colorvertex.shader", "shaders/colorfragment.shader");

	gluPerspective(60 * (float)SCREENWIDTH / SCREENHEIGHT, (float)SCREENWIDTH / SCREENHEIGHT, 0.01f, 100.0f);

	glUseProgram(colorProgramID);

	matrixID = glGetUniformLocation(colorProgramID, "MVP");

	sheet1 = new TextureAtlas("textures/image.png", 8);
	sheet1->TextureID = glGetUniformLocation(colorProgramID, "myTextureSampler");

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);

	rendermode = TEXTURE;

	return true;
}

void Graphics::DrawStaticRect(float x, float y, float w, float h) {
	if (rendermode == TEXTURE) {
		glUseProgram(staticProgramId);
		rendermode = SHAPE;
	}

	const GLfloat vertexdata[]{
		x, y, 0.0f,
		x + w, y, 0.0f,
		x, y + h, 0.0f,

		x + w, y, 0.0f,
		x, y + h, 0.0f,
		x + w, y + h, 0.0f,
	};

	const GLfloat fragmentdata[]{
		x, y, 0.0f,
		x + w, y, 0.0f,
		x, y + h, 0.0f,

		x + w, y, 0.0f,
		x, y + h, 0.0f,
		x + w, y + h, 0.0f,
	};

	GLuint vertexbuffer;
	GLuint colorbuffer;

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexdata), vertexdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fragmentdata), fragmentdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glDeleteProgram(colorProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLuint Graphics::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint Graphics::loadDDS(const char* path) {
	// lay out variables to be used
	unsigned char* header;

	unsigned int width;
	unsigned int height;
	unsigned int mipMapCount;

	unsigned int blockSize;
	unsigned int format;

	unsigned int w;
	unsigned int h;

	unsigned char* buffer = 0;

	// prepare some variables
	unsigned int offset = 0;
	unsigned int size = 0;

	GLuint tid = 0;

	// open the DDS file for binary reading and get file size
	FILE* f;
	if ((f = fopen(path, "rb")) == 0)
		return 0;
	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	// allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
	// read in 128 bytes from the file
	header = (unsigned char*)malloc(128);
	fread(header, 1, 128, f);

	// compare the `DDS ` signature
	if (memcmp(header, "DDS ", 4) != 0)
		goto exit;

	// extract height, width, and amount of mipmaps - yes it is stored height then width
	height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
	width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
	mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);

	// figure out what format to use for what fourCC file type it is
	// block size is about physical chunk storage of compressed data in file (important)
	if (header[84] == 'D') {
		switch (header[87]) {
		case '1': // DXT1
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blockSize = 8;
			break;
		case '3': // DXT3
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			blockSize = 16;
			break;
		case '5': // DXT5
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			blockSize = 16;
			break;
		case '0': // DX10
			// unsupported, else will error
			// as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
			// so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
		default: goto exit;
		}
	}
	else // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
		goto exit;

	// allocate new unsigned char space with file_size - (file_code + header_size) magnitude
	// read rest of file
	buffer = (unsigned char*)malloc(file_size - 128);
	if (buffer == 0)
		goto exit;
	fread(buffer, 1, file_size, f);

	// prepare new incomplete texture
	glGenTextures(1, &tid);
	if (tid == 0)
		goto exit;

	// bind the texture
	// make it complete by specifying all needed parameters and ensuring all mipmaps are filled
	glBindTexture(GL_TEXTURE_2D, tid);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1); // opengl likes array length of mipmaps
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	w = width;
	h = height;

	// loop through sending block at a time with the magic formula
	// upload to opengl properly, note the offset transverses the pointer
	// assumes each mipmap is 1/2 the size of the previous mipmap
	for (unsigned int i = 0; i < mipMapCount; i++) {
		if (w == 0 || h == 0) { // discard any odd mipmaps 0x1 0x2 resolutions
			mipMapCount--;
			continue;
		}
		size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, w, h, 0, size, buffer + offset);
		offset += size;
		w /= 2;
		h /= 2;
	}
	// discard any odd mipmaps, ensure a complete texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	// easy macro to get out quick and uniform (minus like 15 lines of bulk)
exit:
	free(buffer);
	free(header);
	fclose(f);
	return tid;
}
#include "../Classes/TextureAtlas.h"

void TextureAtlas::loadFromImage(const sf::Image& i) {
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i.getSize().x, i.getSize().y,
		0, GL_RGBA, GL_UNSIGNED_BYTE, i.getPixelsPtr());

	glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_MIPMAP);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
}

void TextureAtlas::LoadPNG(const std::string& file) {
	sf::Image i;
	if (!i.loadFromFile(file) && false) {
		throw std::runtime_error("Unable to load BasicTexture: " + file);
	}
	width = i.getSize().x;
	height = i.getSize().y;
	loadFromImage(i);
}

misc::uvcoords TextureAtlas::GetUvCoords(int x, int y) {
	misc::uvcoords uvcoords;
	float factor = (float)resolution / (float)width;

	uvcoords.ul.x = x * factor;
	uvcoords.ul.y = y * factor;

	uvcoords.ur.x = (x * factor) + factor;
	uvcoords.ur.y = y * factor;

	uvcoords.dl.x = x * factor;
	uvcoords.dl.y = (y * factor) + factor;

	uvcoords.dr.x = (x * factor) + factor;
	uvcoords.dr.y = (y * factor) + factor;

	return uvcoords;
}

GLuint TextureAtlas::LoadDDS(const char* path) {
	// lay out variables to be used
	std::cout << "wrong\n";
	unsigned int w;
	unsigned int h;

	unsigned char* buffer = 0;

	// prepare some variables
	unsigned int offset = 0;
	unsigned int size = 0;

	GLuint tid = 0;

	// open the DDS file for binary reading and get file size
	FILE* f;
	if ((f = fopen(path, "rb")) == 0) {
		std::cout << "wrong open\n";
		return 0;
	}
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
		default:
			std::cout << "wrong exit\n";
			goto exit;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
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

GLuint TextureAtlas::LoadBMP(const char* path) {

	printf("Reading image %s\n", path);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE* file = fopen(path, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", path);
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		//fclose(file);
		//return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		//fclose(file);
		//return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) //{ printf("Not a correct BMP file\n");    fclose(file); return 0; }
	if (*(int*)&(header[0x1C]) != 24) //{ printf("Not a correct BMP file\n");    fclose(file); return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];
	std::cout << "image " << imageSize << "\n";

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

std::array<misc::uvcoords, 6> TextureAtlas::GetBlockCoords(int id) {
	std::array<misc::uvcoords, 6> uvcoords{};
	switch (id) {
	case 1:
		uvcoords[0] = GetUvCoords(0, 0);
		uvcoords[1] = GetUvCoords(0, 0);
		uvcoords[2] = GetUvCoords(0, 0);
		uvcoords[3] = GetUvCoords(0, 0);
		uvcoords[4] = GetUvCoords(0, 0);
		uvcoords[5] = GetUvCoords(0, 0);
		break;
	case 2:
		uvcoords[0] = GetUvCoords(1, 0);
		uvcoords[1] = GetUvCoords(1, 0);
		uvcoords[2] = GetUvCoords(0, 0);
		uvcoords[3] = GetUvCoords(4, 0);
		uvcoords[4] = GetUvCoords(1, 0);
		uvcoords[5] = GetUvCoords(1, 0);
		break;
	case 3:
		uvcoords[0] = GetUvCoords(7, 0);
		uvcoords[1] = GetUvCoords(7, 0);
		uvcoords[2] = GetUvCoords(7, 0);
		uvcoords[3] = GetUvCoords(7, 0);
		uvcoords[4] = GetUvCoords(7, 0);
		uvcoords[5] = GetUvCoords(7, 0);
		break;
	}
	return uvcoords;
}

#pragma once
#include <GL/glew.h>

#include <stb_image.h>

class Texture
{
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;

public:
	Texture();

	Texture(const char* fileLoc);

	bool LoadTexture();		// Incarca texturile care nu au canal alpha
	bool LoadTextureA();	// Incarca texturile care au canal alpha

	void UseTexture();
	void ClearTexture();

	~Texture();
};


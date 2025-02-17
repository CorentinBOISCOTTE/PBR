#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& _filename);
	void Load();
	void Unload() const;
	GLuint textureID = NULL;
private:
	std::string filename;
};
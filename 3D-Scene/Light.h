#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
protected:
	//Pentru Ambient lightning
	glm::vec3 colour;
	GLfloat ambientIntensity; // Cat de intensa e lumina: foarte luminos / intunecat 
	
	//Pentru Diffuse lightning
	glm::vec3 direction;
	GLfloat diffuseIntensity;

public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
		  GLfloat aIntensity, GLfloat dIntensity);

	~Light();
};


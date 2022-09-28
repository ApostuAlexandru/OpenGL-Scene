#pragma once

#include <GL/glew.h>

class Material
{
private:
	GLfloat specularIntensity; // Cata lumina absoarbe din sursa de lumina tot obiectul
	GLfloat shininess; // shininess mic = obiect lumina pe o suprafata mai mare | shininess mare = un punct foarte shiny pe obiect
	


public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();
};


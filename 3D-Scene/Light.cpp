#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // Lumina basic va fi alba, simpla
	ambientIntensity = 1;

	direction = glm::vec3(0.0f, -1.0f, 0.0f); // By default, lumina va fi orientata drept in jos
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue); // Lumina basic va fi alba, simpla
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light()
{

}
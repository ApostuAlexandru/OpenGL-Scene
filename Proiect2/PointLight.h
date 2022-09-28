#pragma once
#include "Light.h"

class PointLight :
    public Light
{
public:

    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    ~PointLight();

protected:
    
    glm::vec3 position; // Unde e pozitionata sursa de lumina

    GLfloat constant, linear, exponent; // Coeficientii ecuatiei de gradul 2 de la numitor (pentru factorul de atenuare)

};


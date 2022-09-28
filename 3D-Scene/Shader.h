#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition, // World-Based Values
		   uniformSpecularIntensity, uniformShininess; // Material Values

	struct {
		GLuint uniformColour;				// Ambiental Light Values
		GLuint uniformAmbientIntensity;		// Ambiental Light Values
		GLuint uniformDiffuseIntensity;		// Diffuse Light Values

		GLuint uniformDirection;			// Diffuse Light Values

	}uniformDirectionalLight;

	GLuint uniformPointLightCount; // Unde il tinem minte pe pointLightCount

	struct {
		GLuint uniformColour;				// Ambiental Light Values
		GLuint uniformAmbientIntensity;		// Ambiental Light Values
		GLuint uniformDiffuseIntensity;		// Diffuse Light Values

		GLuint uniformPosition;			// Point Light Values
		GLuint uniformConstant;			// Point Light Values
		GLuint uniformLinear;			// Point Light Values
		GLuint uniformExponent;			// Point Light Values

	}uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct { 
		GLuint uniformColour;				// Ambiental Light Values
		GLuint uniformAmbientIntensity;		// Ambiental Light Values
		GLuint uniformDiffuseIntensity;		// Diffuse Light Values

		GLuint uniformPosition;			// Spot Light Values
		GLuint uniformConstant;			// Spot Light Values
		GLuint uniformLinear;			// Spot Light Values
		GLuint uniformExponent;			// Spot Light Values

		GLuint uniformDirection;		// Spot Light Values
		GLuint uniformEdge;				// Spot Light Values
	}uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);

	void UseShader();

	void ClearShader();

	~Shader();

};


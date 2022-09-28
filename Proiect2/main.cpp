#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "MyWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "assimp\Importer.hpp"

const float toRadians = 3.14159265f / 180.0f;

MyWindow mainWindow;
std::vector<Mesh*> meshList; // Vectorul de obiecte
std::vector<Shader> shaderList; // Vectorul de shadere
Camera camera;

Texture brick;
Texture dirt;
Texture plain;
Texture sandwall;
Texture sand;
Texture rociaride;
Texture marble;

Material shinyMaterial;
Material dullMaterial;

Model golem;
Model jet;
Model trees;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

float jetAngle = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

glm::mat4 QuatToMatrix(float qw, float qx, float qy, float qz)
{
    glm::mat4 result(1.0f);
    
    // qw = q1 | qx = q2 | qy = q3 | qz = q4

    // Prima linie
    result[0][0] = 1 - 2 * qy * qy - 2 * qz * qz;
    result[0][1] = 2 * qx * qy - 2 * qz * qw;
    result[0][2] = 2 * qx * qz + 2 * qy * qw;
    result[0][3] = 0;

    // A doua linie
    result[1][0] = 2 * qx * qy + 2 * qz * qw;
    result[1][1] = 1 - 2 * qx * qx - 2 * qz * qz;
    result[1][2] = 2 * qy * qz - 2 * qx * qw;
    result[1][3] = 0;

    // A treia linie
    result[2][0] = 2 * qx * qz - 2 * qy * qw;
    result[2][1] = 2 * qy * qz + 2 * qx * qw;
    result[2][2] = 1 - 2 * qx * qx - 2 * qy * qy;
    result[2][3] = 0;

    // A patra linie
    result[3][0] = 0;
    result[3][1] = 0;
    result[3][2] = 0;
    result[3][3] = 1;

    return result;
}

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
    //    X      Y     Z              U    V             Nx     Ny   Nz
        -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,         0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,		    0.5f, 0.0f,         0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f,		    1.0f, 0.0f,         0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,		    0.5f, 1.0f,         0.0f, 0.0f, 0.0f
    };

    // ===== Creare si indexare suprafata =====

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -20.0f, 0.0f, -20.0f,      0.0f, 0.0f,          0.0f, -1.0f, 0.0f,
         20.0f, 0.0f, -20.0f,      20.0f, 0.0f,         0.0f, -1.0f, 0.0f,
        -20.0f, 0.0f,  20.0f,      0.0f, 20.0f,         0.0f, -1.0f, 0.0f,
         20.0f, 0.0f,  20.0f,      20.0f, 20.0f,        0.0f, -1.0f, 0.0f
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* piramida1 = new Mesh();
    piramida1->CreateMesh(vertices, indices, 32, 12);        // Primul numar e numaru de varfuri pe care le citim(se adauga si coord de texturare), 
    meshList.push_back(piramida1);                           // al doilea e numarul de indici(3 pt fiecare triunghi = 12)

    Mesh* piramida2 = new Mesh();
    piramida2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(piramida2);

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(floor);

}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = MyWindow(1366, 768); // 1280 x 1024 | 1024 x 768
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders(); 

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.5f);

    // ===== Texturare ===== //
    brick = Texture("Textures/brick.png");
    brick.LoadTextureA();
    dirt = Texture("Textures/dirt.png");
    dirt.LoadTextureA();
    stbi_set_flip_vertically_on_load(true);
    plain = Texture("Textures/plain.png");
    plain.LoadTextureA();
    sand = Texture("Textures/sand_2.png");
    sand.LoadTextureA();
    rociaride = Texture("Textures/rociaride.jpg");
    rociaride.LoadTextureA();
    marble = Texture("Textures/marble.png");
    marble.LoadTextureA();

    // ===== Implementare modele ===== //
    golem = Model();
    golem.LoadModel("Models/golem.obj");

    jet = Model();
    jet.LoadModel("Models/jet.x3d");
   
    trees = Model();
    trees.LoadModel("Models/trees9.obj");

    shinyMaterial = Material(1.0f, 64);
    dullMaterial = Material(0.3f, 4);

    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,     // Parametri Culoare Ambient Lightning
                                 0.2f, 0.2f,           // Parametri Intensitati Ambient si Diffuse Lightning
                                 0.0f, 0.0f, -1.0f);   // Directie Diffuse Lightning

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,       // Parametri Culoare Point Light
                                0.5f, 0.1f,             // Parametri Intensitati Ambient si Diffuse Lightning
                                0.0f, 5.0f, 0.0f,       // Parametri Locatie Point Light
                                0.3f, 0.2f, 0.1f);      // Parametri ax^2 + bx + c -> Factorul de atenuare
    pointLightCount++;

    pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,       // Parametri Culoare Point Light
                                0.5f, 0.1f,             // Parametri Intensitati Ambient si Diffuse Lightning
                                7.0f, 3.5f, -5.0f,      // Parametri Locatie Point Light
                                0.3f, 0.1f, 0.1f);      // Parametri ax^2 + bx + c -> Factorul de atenuare
    pointLightCount++;

    pointLights[2] = PointLight(1.0f, 0.0f, 1.0f,       // Parametri Culoare Point Light
                                0.5f, 0.1f,             // Parametri Intensitati Ambient si Diffuse Lightning
                               -7.0f, 3.5f, -5.0f,      // Parametri Locatie Point Light
                                0.3f, 0.1f, 0.1f);      // Parametri ax^2 + bx + c -> Factorul de atenuare
    pointLightCount++;

    unsigned int spotLightCount = 0;
    
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
						0.0f, 2.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	//spotLightCount++;
    

    // Matricea de proiectie
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
           uniformSpecularIntensity = 0, uniformShininess = 0;
    glm::mat4 projection = glm::perspective(45.0f * toRadians, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Testare Assimp
    Assimp::Importer importer;

    // Quaternion
    glm::mat4 quat_matrix(1.0f);
    quat_matrix = QuatToMatrix(glm::sqrt(2) / 2, 0, -glm::sqrt(2) / 2, 0);
    
    for (int i = 0; i < 4; i++)
    {
        //std::cout << "\n";
        for (int j = 0; j < 4; j++)
        {
            //std::cout << quat_matrix[i][j] << " ";
        }
    }

    glm::mat4 quat_matrix_invert(1.0f);

    for (int i = 0; i < 4; i++)
    {
        //std::cout << "\n";
        for (int j = 0; j < 4; j++)
        {
            //std::cout << quat_matrix_invert[i][j] << " ";
        }
    }

    quat_matrix_invert = QuatToMatrix(glm::sqrt(2) / 2, 0, glm::sqrt(2) / 2, 0);

    // Loop-ul pentru window-ul creat 
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // Acceptare User input
        glfwPollEvents();

        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear Window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glm::vec3 lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


        // ===== Desenare piramida 1 ===== //
        glm::mat4 model(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        rociaride.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh(); 

        // ===== Desenare piramida de 2 ===== //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(7.0f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
        marble.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        // ===== Desenare piramida de 3 ===== //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.0f, 0.0f, -5.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model = glm::rotate(model, -15.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        marble.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        // ===== Desenare suprafata plan ===== //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        sand.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        // ===== Incarcare model copaci ===== //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 12.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        trees.RenderModel();

        // ===== Incarcare model golem ===== //
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, -12.0f));
        model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        golem.RenderModel();
        
        // ===== Incarcare model jet ===== //
        jetAngle += 0.2f;
        if (jetAngle > 360.0f)
        {
            jetAngle = 0.1f;
        }
        model = glm::mat4(1.0f);
        model = glm::rotate(model, jetAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-10.0f, 4.0f, 0.0f));

        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    
        // Rotire folosind quaternioni. Vrem sa rotim pe u = (0,1,0) cu -90 grade 
        // Cuaternionul folosit e q( sqrt(2)/2, 0, -sqrt(2)/2, 0 )

        //model = quat_matrix * model * quat_matrix_invert;

        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        jet.RenderModel();

        glUseProgram(0);

        mainWindow.swapBuffers();

    }

    return 0;
}

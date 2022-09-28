# 3D Scene Using OpenGL

## Description

The purpose of this project is rendering a 3D complex scene, using various concepts such as drawing objects using indexes, texturing, Phong lighting model, importing objects etc. 

The scene is represented by a surface on which a sand texture has been applied and a few pyramids drawn using vertexes through which 3D texturing was exemplified. Moreover, the Phong lighting model can be seen here, as every top of the pyramid is a Point Light, a source of illumination. Last but not least, there have been added a textured plane model which rotates around the origin, a golem model who was scaled (and textured) and some trees.

## Elements included

* 3D objects drawing (using VBO indexing)
* Transformations (Translate, Scale, Rotate)
* Texturing
* Illumination (Phong Model)
* Complex illumination (Point Light + Spot Light)
* Model importing (.obj)
* User-responsive camera

## Screenshots

![rsz_11picture1](https://user-images.githubusercontent.com/18242948/192778844-9ff623ac-e152-4c9c-a57a-31e97f8a3545.png)
![rsz_1picture2](https://user-images.githubusercontent.com/18242948/192778971-24ab1ac9-de57-4c36-9792-d1cd79fb907c.png)

## Libraries used

Besides OpenGL and standard C++ libraries, I also used:

* GLEW
* GLFW - the alternative to GLUT, since GLUT is not supported anymore
* GLM
* Assimp - Compiled with CMake to import 3D Models

## Project structure

![rsz_picture3](https://user-images.githubusercontent.com/18242948/192784380-8009a1aa-de28-44b8-b6ce-d9e77bef65f1.png)

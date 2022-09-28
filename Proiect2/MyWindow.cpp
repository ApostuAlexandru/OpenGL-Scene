#include "MyWindow.h"

MyWindow::MyWindow()
{
	width = 800;
	height = 600;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int MyWindow::Initialise()
{
    // Initializare GLFW
    if (!glfwInit())
    {
        printf("GLFW nu s-a putut initializa!");
        glfwTerminate();
        return 1;
    }

    // Proprietati pentru fereastra GLFW
    // Versiunea OpenGL - In caz de nu merge 4.6, decomenteaza liniile urmatoare
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Pentru incompatibilitate cu versiunile vechi de OpenGL:
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Pentru compatibilitate cu versiunile mai noi de OpenGL;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Proiect 2", NULL, NULL);
    if (!mainWindow)
    {
        printf("Nu s-a putut initializa fereastra GLFW!");
        glfwTerminate();
        return 1;
    }

    // Dimensiunea pentru buffer
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Setup GLEW pentru fereastra anterioara
    glfwMakeContextCurrent(mainWindow);

    // Control Keys + Mouse input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Daca vrem sa nu vedem cursorul, insa sa fie locked in window

    // Pentru proprietati mai noi (optional)
    glewExperimental = GL_TRUE;

    // Verificare initilizare GLEW
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW nu s-a putut initializa !");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Depth
    glEnable(GL_DEPTH_TEST);

    // Creare Viewport (locul unde desenam)
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
}

void MyWindow::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat MyWindow::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat MyWindow::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void MyWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key > 0 && key <= 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            //printf("S-a apasat tasta: %d\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            //printf("S-a eliberat tasta: %d\n", key);
        }
    }
}

void MyWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; // Ordinea e inversa ca sa evitam inverted controls

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    //printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

MyWindow::~MyWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
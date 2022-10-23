#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../utils/Utils.h"


#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

void init(GLFWwindow* window) 
{

    //"assets/vertShader.glsl"
    //"assets/fragShader.glsl"
    renderingProgram = Utils::createShaderProgram("assets/vertShader.glsl", "assets/fragShader.glsl");

    //prepare data to send down pipeline
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

float x = 0.0f;
float inc = 0.01f;

void display(GLFWwindow* window, double currentTime)
{
    //GPU works!

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // just load a programs on GPU
    glUseProgram(renderingProgram);

    // animation start

    x += inc;
    if(x > 1.0f)
    {
        inc = -0.01f;
    }

    if(x < -1.0f)
    {
        inc = 0.01f;
    }

    // get uniform location
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");

    // set uniform value by location
    glProgramUniform1f(renderingProgram, offsetLoc, x);

    // animation end

    // say how many times vertex shader should be runned
    auto verticesCount = 3;

    // initiate pipeline processing
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}

int main(void)
{
    if(!glfwInit()) 
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 2- program 1", NULL, NULL);

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    init(window);

    while(!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

}
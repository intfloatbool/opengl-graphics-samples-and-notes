#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../utils/Utils.h"

#define numVAOs 1
#define numVBOs 2

float cameraX;
float cameraY;
float cameraZ;

float cubeLocX;
float cubeLocY;
float cubeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Allocate variables used in display() function, so they won't need to be allocated during rendering

GLuint mvLoc;
GLuint vLoc;
GLuint projLoc;
GLuint tfLoc;
GLuint mLoc;

int width;
int height;
float aspect;

glm::mat4 pMat;
glm::mat4 vMat;
glm::mat4 mMat;
glm::mat4 mvMat;

glm::mat4 rMat;
glm::mat4 tMat;

// 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin 
void setupVertices(void)
{
    float vertexPositions[128] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    // reduce sizes
    for(int i = 0; i < 128; i++)
    {
        vertexPositions[i] *= 0.25f;
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("assets/vertShader.glsl", "assets/fragShader.glsl");

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;

    cubeLocX = 0.0f;
    cubeLocY = -2.0f;
    cubeLocZ = 0.0f;

    setupVertices();

}

void display(GLFWwindow* window, double currentTime)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    // enable shaders
    glUseProgram(renderingProgram);

    // get the uniform variables for the MV and projection matrices
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
    // build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float) width / (float) height;
    float radians = 1.0472f; //60 degrees
    float near = 0.1f;
    float far = 1000.0f;

    for(int i = 0; i < 24; i++)
    {
        pMat = glm::perspective(radians, aspect, near, far);

        // build view matrix, model matrix and model-view matrix
        vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
        mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

        // copy perspective and MV matrices to corresponding uniform variables
        
        glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

        tfLoc = glGetUniformLocation(renderingProgram, "tf");

        float timeFactor = (float) currentTime;
        glUniform1f(tfLoc, timeFactor);

        // associate VBO with the corresponding vertex attribute in the vertex shader

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


        // arg takes from vertShader 'location' : layout (location = 0) in vec3 position;
        glEnableVertexAttribArray(0);

        // adjust OpenGL settings and draw model
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    // to draw once
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);

}

int main(void) { // main() is unchanged from before
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - program 1", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);
    init(window);
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
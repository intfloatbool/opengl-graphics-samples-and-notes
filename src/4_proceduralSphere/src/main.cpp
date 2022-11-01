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
#include "Sphere.hh"

using namespace std;
#define numVAOs 1
float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[3];

GLuint brickTexture;

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
float lastFrameTime = 0;
float currentAngle = 0;

Sphere mySphere(128);



void setupVertices(void) 
{ 
    auto ind = mySphere.getIndices();
    auto vert = mySphere.getVertices();
    auto tex = mySphere.getTextureCoords();
    auto norm = mySphere.getNormals();

    std::vector<float> pValues; // vertex positions
    std::vector<float> tValues; // texture coordinates
    std::vector<float> nValues; // normal vectors

    int numIndices = mySphere.getNumIndices();

    for(int i = 0; i < numIndices; i++)
    {
        auto index = ind[i];
        pValues.push_back((vert[index]).x);
        pValues.push_back((vert[index]).y);
        pValues.push_back((vert[index]).z);

        tValues.push_back(tex[index].x);
        tValues.push_back(tex[index].y);

        nValues.push_back(norm[index].x);
        nValues.push_back(norm[index].y);
        nValues.push_back(norm[index].z);
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(3, vbo);


    // push vertices into buffer #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pValues.size() * 4, &pValues[0], GL_STATIC_DRAW);

    // push texture coordinates into buffer #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tValues.size() * 4, &tValues[0], GL_STATIC_DRAW);

    // push normals into buffer #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nValues.size() * 4, &nValues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) 
{
    renderingProgram = Utils::createShaderProgram("assets/vertShader.glsl", "assets/fragShader.glsl");

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;

    pyrLocX = 0.0f;
    pyrLocY = 0.0f;
    pyrLocZ = 4.0f;

    setupVertices();

    brickTexture = Utils::loadTexture("assets/bricks.jpg");
}

void display(GLFWwindow* window, double currentTime) 
{

    float deltaTime = currentTime - lastFrameTime;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderingProgram);
    // get the uniform variables for the MV and projection matrices
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    // build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
    // build view matrix, model matrix, and model-view matrix
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    // draw the pyramid (use buffer #0)
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));
    
    float speed = 1.0f;

    currentAngle += speed * deltaTime;

    mMat = glm::rotate(mMat, currentAngle, glm::vec3(0, 1, 0));

    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

    lastFrameTime = currentTime;
}

int main(void) 
{ 
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
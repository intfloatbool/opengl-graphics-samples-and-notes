#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <fstream>

#include "../../libs/soil2/include/SOIL2.h"

class Utils
{
    public:
        static std::string readShaderSource(const char* filePath);
        static bool checkOpenGLError();
        static GLuint createShaderProgram(const char* vertShader, const char* fragShader);
        static void printShaderLog(GLuint shader);
        static void printProgramLog(int prog);
        static GLuint loadTexture(const char* texImagePath);
};
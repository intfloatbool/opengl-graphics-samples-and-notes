#include "Utils.h"

std::string Utils::readShaderSource(const char* filePath)
{
    std::ifstream ifs(filePath);

    std::string content((std::istreambuf_iterator<char>(ifs)),
        (std::istreambuf_iterator<char>()));

    if (!ifs) 
    {
        std::cerr << "File at path '" << filePath << "'" << " is not loaded!" << std::endl;
    }
    

    return content;
}

bool Utils::checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();

    while(glErr != GL_NO_ERROR)
    {
        std::cout << "GL Error: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }

    return foundError;
}

GLuint Utils::createShaderProgram(const char* vertShaderPath, const char* fragShaderPath)
{
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    std::string vertexShaderCode = readShaderSource(vertShaderPath);
    std::string fragmentShaderCode = readShaderSource(fragShaderPath);

    const char* vShaderSource = vertexShaderCode.c_str();

    const char* fShaderSource = fragmentShaderCode.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSource, NULL);
    glShaderSource(fShader, 1, &fShaderSource, NULL);
    
    // catch errors while compiling shaders
    glCompileShader(vShader);

    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if(vertCompiled != 1)
    {
        std::cout << "Vertex shader compilation failed" << std::endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);

    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if(fragCompiled != 1)
    {
        std::cout << "Fragment shader compilation failed" << std::endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();

    // Catch errors while linking shaders
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);

    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1)
    {
        std::cout << "linking failed" << std::endl;
        printProgramLog(vfProgram);
    }



    return vfProgram;
}

void Utils::printShaderLog(GLuint shader)
{
    int len = 0;
    int chWrittn = 0;

    char* log;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    if(len > 0)
    {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader info log: " << log << std::endl;
        free(log);
    }
}

void Utils::printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;

    char* log;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

    if(len > 0)
    {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        std::cout << "Program info log: " << log << std::endl;
        free(log);
    }   
}

GLuint Utils::loadTexture(const char* texImagePath)
{
    GLuint textureId;

    textureId = SOIL_load_OGL_texture(texImagePath, 
        SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if(textureId == 0)
    {
        std::cout << "could not find texture file at path: " << texImagePath << std::endl;
    }

    return textureId;
}
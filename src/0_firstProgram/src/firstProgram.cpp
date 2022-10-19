#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <string>
#include <fstream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

std::string readShaderSource(const char* filePath)
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

void printShaderLog(GLuint shader)
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

void printProgramLog(int prog)
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

bool checkOpenGLError()
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

GLuint createShaderProgram()
{

    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    std::string vertexShaderCode = readShaderSource("assets/vertShader.glsl");
    std::string fragmentShaderCode = readShaderSource("assets/fragShader.glsl");

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

void init(GLFWwindow* window) 
{
    renderingProgram = createShaderProgram();

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
// OpenGL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL Math and terminal includes
#include <glm.hpp>
#include <iostream>

// File reading includes
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

// Special functions

// file reader ---
std::string loadShaderSource(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open shader: " + filepath);

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
// --- file reader

// shader reader ---
GLuint compileShader(GLenum type, const std::string &filepath)
{
    std::string src = loadShaderSource(filepath);
    const char *srcPtr = src.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &srcPtr, nullptr); // nullptr = null-terminated
    glCompileShader(shader);

    // Check for errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        throw std::runtime_error("Shader compile error (" + filepath + "):\n" + log);
    }

    return shader;
}

GLuint createShaderProgram(const std::string &vertPath, const std::string &fragPath)
{
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertPath);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragPath);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // Check link errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        throw std::runtime_error("Program link error:\n" + std::string(log));
    }

    // Shaders are linked — no longer needed
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}
// --- shader reader

// window defenitions
static int screen_width = 800;
static int screen_height = 800;
static char window_name[] = "Learning OpenGl window";

using namespace std;

int main()
{
    glfwInit(); // initilize glfw

    // include the shaders
    GLuint shaderProgram = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    // use shaders
    glUseProgram(shaderProgram);

    // specify OpenGl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, window_name, NULL, NULL); // no specification for "*monitor" and "*share"
    if (window == NULL)                                                                          // if the window does not exist
    {
        cout << "Failed to create a window" << endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // make "window" part of the current context

    gladLoadGL(); // start up glad

    // specify the veiwport of the window
    glViewport(0, 0, screen_width, screen_height);

    // set the bacground color
    glClearColor(0.07, 0.13, 0.17, 1);

    // replace the old background color
    glClear(GL_COLOR_BUFFER_BIT);

    // update display
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) // if the "X" is not pressed..
    {
        glfwPollEvents(); // process all events
    }

    glfwDestroyWindow(window); // close the window;
    glfwTerminate();           // end glfw
    return 0;
}
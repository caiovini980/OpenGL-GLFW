/*
 *  OPENGL DOCUMENTATION: https://docs.gl/
 *  Program made by: Caio Aguiar
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// #include "../LegacyOpenGL/DebugMethods.h"

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT,
    };
    
    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;
    
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }

            continue;
        }

        stringStream[static_cast<int>(type)] << line << " \n";
    }

    return { stringStream[0].str(), stringStream[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    // Shader Id
    // Number of source codes
    // Pointer to the C string source code
    // The length of source code in case we are providing multip
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << "Message: " << message << "\n";

        glDeleteShader(id);
        
        return 0;
    }
    
    return id;
}

// Takes source code of each shader and compile into shaders
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach compiled shaders to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // link and validate the program
    glLinkProgram(program);
    glValidateProgram(program);

    // clear used shaders from program
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void SetupVertexPositions()
{
    constexpr float positions[6] = {
        -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    
    // define buffer (give data to OpenGL)
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
    // Setup Attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    
    // Enable Attribute
    glEnableVertexAttribArray(0);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Init GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "ERROR: Glew init is invalid!\n";
        return -1;
    }
    
    // Define POSITION
    SetupVertexPositions();

    const std::string shaderPath = "./Resources/Shaders/Basic.shader";
    const ShaderProgramSources sources = ParseShader(shaderPath);

    std::cout << "VERTEX:" << "\n";
    std::cout << sources.VertexSource << "\n";
    std::cout << "FRAGMENT:" << "\n";
    std::cout << sources.FragmentSource << "\n";
    
    const unsigned int shader = CreateShader(sources.VertexSource, sources.FragmentSource);
    glUseProgram(shader);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

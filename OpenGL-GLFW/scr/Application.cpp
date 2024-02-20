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
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Vendor/imgui.h"
#include "Vendor/imgui_impl_glfw.h"
#include "Vendor/imgui_impl_opengl3.h"

// #include "../LegacyOpenGL/DebugMethods.h"

// Takes source code of each shader and compile into shaders

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const char* glsl_version = "#version 330";
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // number of screen updates to wait from the time glfwSwapBuffers was called before swapping the buffers and returning
    glfwSwapInterval(5);

    // Init GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "ERROR: Glew init is invalid!\n";
        return -1;
    }

    {
        // Define POSITION
        constexpr float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,// 0
             50.0f, -50.0f, 1.0f, 0.0f,// 1
             50.0f,  50.0f, 1.0f, 1.0f,// 2
            -50.0f,  50.0f, 0.0f, 1.0f,// 3

            // -10.5f, -10.5f, 0.0f, 0.0f,
            // 10.5f, -10.5f, 1.0f, 0.0f,
            // 10.5f, 10.5f, 1.0f, 1.0f,
            // -10.5f, 10.5f, 0.0f, 1.0f,

            // -0.5f, -0.5f, 0.0f, 0.0f,
            // 0.5f, -0.5f, 1.0f, 0.0f,
            // 0.5f, 0.5f, 1.0f, 1.0f,
            // -0.5f, 0.5f, 0.0f, 1.0f,
        };
        
        // define buffer (give data to OpenGL)
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Blending
        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
        
        // Setup Buffers
        // VERTEX
        VertexArray vertexArray;
        const VertexBuffer vertexBuffer{ positions, 4 * 4 * sizeof(float) };
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, layout);

        // INDEX
        const IndexBuffer indexBuffer { indices, 6 };

        // Projection Matrix (ASPECT RATIO)
        // View Matrix (CAMERA TRANSFORM)
        // 
        glm::mat4x4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4x4 view = glm::translate(glm::mat4x4(1.0f), glm::vec3(0, 0, 0));

        // Shader
        std::string shaderPath = "./Resources/Shaders/Basic.shader";
        std::string texturePath = "./Resources/Textures/KokkuLogo.png";
        const std::string textureName = "u_Texture";
        const std::string projectionName = "u_MVP";
        int slot = 0;
        
        Shader shader {std::move(shaderPath)};
        Texture texture {std::move(texturePath)};
        shader.Bind();
        texture.Bind(slot);
        
        shader.SetUniform1i(textureName, slot);
        
        // unbind everything
        vertexArray.Unbind();
        indexBuffer.Unbind();
        vertexBuffer.Unbind();
        shader.Unbind();

        Renderer renderer;
        
        // Setup ImGUI
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        
        float red = 0.0f;
        float increment = 0.05f;
        bool isDarkMode = true;
        
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            {
                glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), translationA);
                glm::mat4x4 mvp = projection * view * model;
                shader.Bind();
                shader.SetUniformMatrix4f(projectionName, mvp);
                
                renderer.Draw(vertexArray, indexBuffer, shader);
            }
            
            {
                glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), translationB);
                glm::mat4x4 mvp = projection * view * model;
                shader.Bind();
                shader.SetUniformMatrix4f(projectionName, mvp);
                
                renderer.Draw(vertexArray, indexBuffer, shader);
            }

            if (red > 1.0f)
            {
                increment = - 0.05f;
            }
            else if (red < 0.0f)
            {
                increment = 0.05f;
            }

            red += increment;

            {

                ImGui::Begin("Objects translations");
                ImGui::Checkbox("Dark Mode", &isDarkMode);
                
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            if (isDarkMode)
            {
                ImGui::StyleColorsDark();
            }
            else
            {
                ImGui::StyleColorsLight();
            }
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            
            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}

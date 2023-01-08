#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "test/TestClearColor.h"
#include "test/Test.h"


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwSwapInterval(1);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {       

        Renderer renderer;

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
            renderer.Clear();
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            if(currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if(currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGui();
                ImGui::End();
            }
            
          
            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
           
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
            
        }
        delete currentTest;
        if(currentTest != testMenu)
        {
            delete testMenu;
        }
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

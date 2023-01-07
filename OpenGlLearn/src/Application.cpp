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
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,
            50.0f, -50.0f, 1.0f, 0.0f,
           50.0f, 50.0f, 1.0f, 1.0f,
            -50.0f, 50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR));
        VertexArray va;

        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        //Define layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f,  (float)height, -1.0f, 1.0f);
        glm::vec3 translationA(0, 0, 0);
        glm::vec3 translationB(0, 0, 0);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), translationA);
      
        
        Renderer renderer;
        Texture texture("res/textures/sample_texture.png");
        texture.Bind(0);
        GLCall(shader.SetUniform1i("u_Texture", 0 ));
        
        float r = 0.0f;
        float increment = 0.05f;

        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();
            
          

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                ImGui::Begin("Properties");                          // Create a window called "Hello, world!" and append into it.

                ImGui::SliderFloat3("Translate A", &translationA.x, 0.0f, width);
                ImGui::SliderFloat3("Translate B", &translationB.x, 0.0f, width);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            GLClearError();


            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
                
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
         
            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;
            

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
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

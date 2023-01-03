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


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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


    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
            -110.5f, -110.5f, 0.0f, 0.0f,
            110.5f, -110.5f, 1.0f, 0.0f,
            110.5f, 110.5f, 1.0f, 1.0f,
            -110.5f, 110.5f, 0.0f, 1.0f
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

        /*ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);*/

        Renderer renderer;
        Texture texture("res/textures/sample_texture.png");
        texture.Bind(0);
        GLCall(shader.SetUniform1i("u_Texture", 0 ));
        
        float r = 0.0f;
        float increment = 0.05f;

        shader.Unbind();
        vb.Unbind();
        ib.Unbind();


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
             int width, height;
            glfwGetWindowSize(window, &width, &height);
            /* Render here */

            renderer.Clear();

            GLClearError();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f,  (float)height, -1.0f, 1.0f);
            shader.SetUniformMat4f("u_MVP", proj);


            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

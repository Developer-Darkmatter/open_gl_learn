#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <fstream>
#include <string>
#include <sstream>

struct  ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    if (!stream.is_open()) {
        std::cout << "Cannot open file " << filePath << std::endl;
        return {};
    }
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(std::getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                //Set mode to vertex
                type = ShaderType::VERTEX;
            }else if(line.find("fragment") != std::string::npos)
            {
                //Set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    
    return {ss[0].str(), ss[1].str()};
}
static unsigned int CompileShader( unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(malloc(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
   unsigned int program = glCreateProgram();
   unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
   unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

   glAttachShader(program, vs);
   glAttachShader(program, fs);
   glLinkProgram(program);
   glValidateProgram(program);

   glDeleteShader(vs);
   glDeleteShader(fs);

   return program;
}

int main() {
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


    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int vertextBuffer;
    glGenBuffers(1, &vertextBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertextBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),  positions, GL_STATIC_DRAW);
   
    glEnableVertexAttribArray(0);
    //Setting attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");


    std::cout << "Vertex Shader " << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment Shader" <<  std::endl;
    std::cout << source.FragmentSource << std::endl;
    unsigned int  shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glDrawArrays(GL_TRIANGLES, 0, 3 );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

void drawTriangle() {

}
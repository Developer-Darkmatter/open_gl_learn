﻿#pragma once
#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::pmr::unordered_map<std::string, unsigned int> m_LocationCache;

public:
    Shader(const std::string filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;


    //Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const ::std::string& name, glm::mat4& matrix);

private:
    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};

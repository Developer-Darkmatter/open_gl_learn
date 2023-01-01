#pragma once
#include "VertexBuffer.h"
class VertexBufferLayout;
class VertexArray
{
private:
    unsigned int m_RendererId;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexLayout);
    
    void Bind() const;
    void Unbind() const;
    
};

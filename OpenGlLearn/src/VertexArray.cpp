#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererId));
   
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexLayout)
{
    Bind();
    vertexBuffer.Bind();
    const auto& elements = vertexLayout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vertexLayout.GetStride(),reinterpret_cast<const void*>(offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
  
  
}

void VertexArray::Bind()
{
    GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind()
{
    GLCall(glBindVertexArray(0));
}

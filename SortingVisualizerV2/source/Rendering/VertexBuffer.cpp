#include "VertexBuffer.h"
#include "Renderer.h"

/* vertex_buffer::vertex_buffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}*/

//vertex_buffer::~vertex_buffer() { glDeleteBuffers(1, &m_renderer_id); }


void vertex_buffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

#include "IndexBuffer.h"

index_buffer::index_buffer(const void* data, unsigned int count) : m_count(count)
{
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
}

index_buffer::~index_buffer() { glDeleteBuffers(1, &m_renderer_id); }

void index_buffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id); }

void index_buffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

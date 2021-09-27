#pragma once

class vertex_buffer
{
public:
    vertex_buffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    ~vertex_buffer() { glDeleteBuffers(1, &m_renderer_id); }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id); };
    void unbind() const;

private:
    unsigned int m_renderer_id;
};

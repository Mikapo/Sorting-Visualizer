#pragma once

#include "Renderer.h"

class index_buffer
{
public:
    index_buffer(const void* data, unsigned int count);
    ~index_buffer();

    void bind() const;
    void unbind() const;
    inline unsigned int get_count() const { return m_count; }

private:
    unsigned int m_renderer_id;
    unsigned int m_count;
};

#pragma once

#include "VertexBufferLayout.h"

class vertex_array
{
public:

	vertex_array();
    ~vertex_array();

	void add_buffer(const class vertex_buffer& vb, const vertex_buffer_layout& layout);
    void bind() const;
    void unbind() const;

	private:

		unsigned int m_renderer_id;
};
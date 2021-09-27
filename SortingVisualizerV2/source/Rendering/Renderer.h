#pragma once

#include "glew.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class renderer
{
public:
    void draw(const class vertex_array& va, const class index_buffer& ib, const class shader& shader) const;
    void clear() const;
};
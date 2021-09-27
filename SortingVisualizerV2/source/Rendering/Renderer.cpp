#include "Renderer.h"

void renderer::draw(const vertex_array& va, const index_buffer& ib, const shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
}

void renderer::clear() const 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
}

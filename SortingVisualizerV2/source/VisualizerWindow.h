#pragma once

#include "Rendering/Renderer.h"

class visualizer_window
{
public:

	visualizer_window(int width, int height);

	void start_render_loop();

private:


	void render_loop();
    void setup_imgui();
    void setup_window();
	void handle_imgui(class sorter& sorter_instance);
    void cleanup();
    void draw_item(unsigned int position, unsigned int size, unsigned int max_items, const shader& shader);

	renderer m_renderer;
	bool has_started = false;
	struct GLFWwindow* m_window = nullptr;
    int m_width, m_height;
};

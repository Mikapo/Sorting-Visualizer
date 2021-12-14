#include "VisualizerWindow.h"
#include "Sorter.h"

#include "glew.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

visualizer_window::visualizer_window(int width, int height) : m_width(width), m_height(height) {}

void visualizer_window::start_render_loop()
{
    if (!has_started)
    {
        has_started = true;
        render_loop();
    }
}

void visualizer_window::render_loop()
{
    setup_window();
    shader shader("shaders/Shader.shader");

    sorter sorter_instance;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_window))
    {
        m_renderer.clear();
        /* Render here */

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (int i = 0; i < sorter_instance.get_size(); i++)
        {
            if (sorter_instance.get_output().first_cursor_position == i)
                shader.set_uniform4f("u_color", 0, 1, 0, 0);
            else if (sorter_instance.get_output().second_cursor_position == i)
                shader.set_uniform4f("u_color", 1, 0, 0, 0);
            else
                shader.set_uniform4f("u_color", 0.2, 0.3, 0.8, 0);

            draw_item(i, sorter_instance.get_items()[i], sorter_instance.get_size(), shader);
        }

        handle_imgui(sorter_instance);

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    cleanup();
}

void visualizer_window::setup_imgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void visualizer_window::setup_window()
{
    /* Initialize the library */
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(m_width, m_height, "Sort visualizer", NULL, NULL);

    if (!m_window)
    {
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glewInit();
    setup_imgui();
}

void visualizer_window::handle_imgui(class sorter& sorter_instance)
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoMove;
    static bool open = true;
    ImGui::Begin("Sort Visualizer", &open, window_flags);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(300, 225));

    if (ImGui::Button("Randomize"))
        sorter_instance.randomize();

    static int items_amount = 20;
    ImGui::SliderInt("Items", &items_amount, 20, 500);

    if (items_amount != sorter_instance.get_size())
        sorter_instance.set_size(items_amount);

    static int new_delay = 20;
    ImGui::SliderInt("Delay(ms)", &new_delay, 0, 300);

    if (new_delay != sorter_instance.get_delay())
        sorter_instance.set_delay(new_delay);

    static int new_max_size = 75;
    ImGui::SliderInt("Max size", &new_max_size, 2, 100);

    if (new_max_size != sorter_instance.get_max_size())
        sorter_instance.set_max_size(new_max_size);

    const char* items[] = {"SelectionSort",     "InsertionSort", "ShellSort", "MergeSort",
                           "BottonUpMergeSort", "QuickSort",     "HeapSort"};
    static const char* current_item = nullptr;

    if (ImGui::BeginCombo("Sort", current_item))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]);
            if (ImGui::Selectable(items[n], is_selected))
                current_item = items[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (current_item == "SelectionSort")
        sorter_instance.set_sort(sorts::selection_sort);
    else if (current_item == "InsertionSort")
        sorter_instance.set_sort(sorts::insertion_sort);
    else if (current_item == "ShellSort")
        sorter_instance.set_sort(sorts::shell_sort);
    else if (current_item == "MergeSort")
        sorter_instance.set_sort(sorts::merge_sort);
    else if (current_item == "BottonUpMergeSort")
        sorter_instance.set_sort(sorts::botton_up_merge_sort);
    else if (current_item == "QuickSort")
        sorter_instance.set_sort(sorts::quick_sort);
    else if (current_item == "HeapSort")
        sorter_instance.set_sort(sorts::heap_sort);

    static bool reverse_sort = false;
    ImGui::Checkbox("Reverse sort", &reverse_sort);

    if (reverse_sort != sorter_instance.is_reverse_sorting())
        sorter_instance.set_reverse_sort(reverse_sort);

    ImGui::Dummy(ImVec2(10, 30));

    if (ImGui::Button("Start", ImVec2(100, 20)))
        sorter_instance.start_sort();

    ImGui::SameLine();

    if (ImGui::Button("Stop", ImVec2(100, 20)))
        sorter_instance.stop_sort();

    std::string text = "Comparisons: " + std::to_string(sorter_instance.get_output().comparisons) +
                       "     Swaps: " + std::to_string(sorter_instance.get_output().swaps);
    ImGui::Text(text.c_str());

    ImGui::End();
}

void visualizer_window::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void visualizer_window::draw_item(
    unsigned int position, unsigned int size, unsigned int max_items, const shader& shader)
{
    struct point2d
    {
        float X, Y;
    };

    float width = 2.0f / static_cast<float>(max_items);
    point2d first_point = {(width * position) - 1 + 0.5 / max_items, -1};
    point2d second_point = {first_point.X + width - 0.5 / max_items, -1};
    point2d third_point = {first_point.X, (static_cast<float>(size) / 50.0f) - 1};
    point2d fourth_point = {second_point.X, (static_cast<float>(size) / 50.0f) - 1};

    float positions[]{first_point.X,  first_point.Y,  second_point.X, second_point.Y,
                      fourth_point.X, fourth_point.Y, third_point.X,  third_point.Y};
    unsigned int indices[]{0, 1, 2, 2, 3, 0};
    vertex_array va;
    vertex_buffer vb(positions, sizeof(float) * 4 * 2);
    vertex_buffer_layout layout;
    layout.push<float>(2);
    va.add_buffer(vb, layout);

    index_buffer ib(indices, 6);

    m_renderer.draw(va, ib, shader);
}

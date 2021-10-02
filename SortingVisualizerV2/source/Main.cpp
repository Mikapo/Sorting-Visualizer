#include "VisualizerWindow.h"
#include <iostream>

#include "Sorts/HeapSort.h"

int WinMain()
{
    visualizer_window window(1920, 1080);
    window.start_render_loop();
}

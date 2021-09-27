#include "VisualizerWindow.h"

int WinMain()
{
    visualizer_window window(1920,1080);
    window.start_render_loop();
}
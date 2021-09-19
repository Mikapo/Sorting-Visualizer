#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include <string.h>
#include <thread>
#include <vector>
#include "Timer.h"
#include "main.h"

bool finished = false;
int comparisons = 0;
int swaps = 0;
sf::Text swap_text;
sf::Text comparsion_text;
sf::Text timer_text;
mikalib::timer timer;
sf::Font font;

struct RectangleData
{
    RectangleData(float size, sf::RectangleShape shape) : m_size(size), m_shape(shape) {}

    float m_size;
    sf::RectangleShape m_shape;
};

struct InputData
{
    std::vector<float> sizes;
    std::string sort;
    int delay;
};

struct CursorPositionsData
{
    int FirstCursor = 0;
    int SecondCursor = 0;
};

template <typename T>
void swap(std::vector<T>& input, size_t index_1, size_t index_2)
{
    swaps++;
    T temp_value = input[index_1];
    input[index_1] = input[index_2];
    input[index_2] = temp_value;
}

void UpdateVisualisation(
    sf::RenderWindow* window, std::vector<RectangleData>& rectangles, const CursorPositionsData& cursor_positions)
{
    float position = 0;
    for (size_t i = 0; i < rectangles.size(); i++)
    {
        float width = rectangles[i].m_shape.getSize().x;
        float height = rectangles[i].m_shape.getSize().y;

        rectangles[i].m_shape.setPosition(position, 1080 - height / 2);
        position += width;

        if (i == cursor_positions.FirstCursor)
            rectangles[i].m_shape.setFillColor(sf::Color::Green);
        else if (i == cursor_positions.SecondCursor)
            rectangles[i].m_shape.setFillColor(sf::Color::Red);
        else
            rectangles[i].m_shape.setFillColor(sf::Color::Blue);

        window->draw(rectangles[i].m_shape);
    }

    comparsion_text.setString("Comparisons: " + std::to_string(comparisons));
    swap_text.setString("Swaps: " + std::to_string(swaps));
    timer_text.setString("Time: " + std::to_string((int)timer.get_current_time()) + " ms");

    window->draw(comparsion_text);
    window->draw(swap_text);
    window->draw(timer_text);


}

void Render(std::vector<RectangleData>& rectangles, const CursorPositionsData& cursor_positions)
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (!finished)
        {
            window.clear(sf::Color::Black);
            UpdateVisualisation(&window, rectangles, cursor_positions);
            window.display();
        }
    }
}

std::vector<RectangleData> create_rectangles(std::vector<float> sizes, float width)
{
    std::vector<RectangleData> output;
    sf::RectangleShape temp;
    temp.setFillColor(sf::Color::Blue);
    temp.setOutlineColor(sf::Color::Black);

    if (sizes.size() < 1000)
        temp.setOutlineThickness(1);

    for (float size : sizes)
    {
        temp.setSize(sf::Vector2f(width, size));
        output.emplace_back(size, temp);
    }
    return output;
}

template <typename T>
void insertion_sort(
    std::vector<T>& input, CursorPositionsData& CursorPosition, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        CursorPosition.FirstCursor = i;
        for (int a = i; a > 0; a--)
        {
            CursorPosition.SecondCursor = a;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            comparisons++;
            if (!greater_than(input[a], input[a - 1]))
            {
                swap(input, a, a - 1);
            }
            else
                break;
        }
    }
    finished = true;
}

template <typename T>
void shell_sort(
    std::vector<T>& input, CursorPositionsData& CursorPosition, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    int h = 1;
    while (h < size / 3)
        h = 3 * h + 1;

    while (h >= 1)
    {
        for (int i = h; i < size; i++)
        {
            CursorPosition.FirstCursor = i;
            for (int a = i; a >= h; a -= h)
            {
                CursorPosition.SecondCursor = a;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                comparisons++;
                if (!greater_than(input[a], input[a - h]))
                {
                    swap(input, a, a - h);
                }
                else
                    break;
            }
        }
        h = h / 3;
    }

    finished = true;
}

template <typename T>
void selection_sort(
    std::vector<T>& input, CursorPositionsData& CursorPosition, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        CursorPosition.FirstCursor = i;
        size_t smallest_value = i;
        for (int a = i + 1; a < size; a++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            CursorPosition.SecondCursor = a;
            comparisons++;
            if (greater_than(input[smallest_value], input[a]))
                smallest_value = a;
        }
        swap<T>(input, i, smallest_value);
    }
    finished = true;
}

InputData inputs()
{
    InputData input;
    std::size_t amount;
    std::cout << "input size of sorting:" << std::endl;
    std::cin >> amount;
    std::cout << std::endl;

    for (size_t i = 0; i < amount; i++)
    {
        input.sizes.push_back((float)(10 + rand() % 200) * 10);
    }

    std::cout << "input sorting type:" << std::endl;
    std::cout << std::endl;
    std::cout << "available types" << std::endl;
    std::cout << std::endl;
    std::cout << "SelectionSort" << std::endl;
    std::cout << "InsertionSort" << std::endl;
    std::cout << "ShellSort" << std::endl;

    std::string type;
    while (true)
    {
        std::cin >> type;

        if (type == "SelectionSort" || type == "InsertionSort" || type == "ShellSort")
        {
            input.sort = type;
            break;
        }
        std::cout << "wrong type!" << std::endl;
    }

    std::cout << "input delay in ms:" << std::endl;
    std::cin >> input.delay;

    return input;
}

void CreateText()
{
    font.loadFromFile("Roboto.ttf");
    comparsion_text.setFont(font);
    comparsion_text.setCharacterSize(20);
    swap_text.setFont(font);
    swap_text.setCharacterSize(20);
    swap_text.setPosition(0, 25);
    timer_text.setFont(font);
    timer_text.setCharacterSize(20);
    timer_text.setPosition(0, 50);
    timer.start();
}

int main()
{
    
    CreateText();

    InputData input = inputs();
    std::vector<RectangleData> rectangles = create_rectangles(input.sizes, 1900.0f / input.sizes.size());

    auto greater_than = [](RectangleData a, RectangleData b) { return a.m_size > b.m_size; };
    CursorPositionsData CursorPositions;

    std::thread thread;
    if (input.sort == "SelectionSort")
        thread = std::thread(
            selection_sort<RectangleData>, std::ref(rectangles), std::ref(CursorPositions), greater_than, input.delay);
    else if (input.sort == "InsertionSort")
        thread = std::thread(
            insertion_sort<RectangleData>, std::ref(rectangles), std::ref(CursorPositions), greater_than, input.delay);
    else if (input.sort == "ShellSort")
        thread = std::thread(
            shell_sort<RectangleData>, std::ref(rectangles), std::ref(CursorPositions), greater_than, input.delay);

    Render(rectangles, CursorPositions);
}

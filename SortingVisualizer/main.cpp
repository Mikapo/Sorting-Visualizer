
#include "MergeSort.h"
#include "Timer.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string.h>
#include <thread>
#include <vector>

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

int comparisons = 0;
int swaps = 0;
sf::Text sort_text;
sf::Text items_text;
sf::Text swap_text;
sf::Text comparsion_text;
sf::Text timer_text;
mikalib::timer timer;
sf::Font font;
CursorPositionsData cursor_positions;

template <typename T>
void swap(std::vector<T>& input, size_t index_1, size_t index_2)
{
    swaps++;
    T temp_value = input[index_1];
    input[index_1] = input[index_2];
    input[index_2] = temp_value;
}

void UpdateVisualisation(sf::RenderWindow* window, std::vector<RectangleData>& rectangles)
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
    timer_text.setString("Time: " + std::to_string(timer.get_current_time() / 1000) + " s");

    window->draw(sort_text);
    window->draw(items_text);
    window->draw(comparsion_text);
    window->draw(swap_text);
    window->draw(timer_text);
}

void Render(std::vector<RectangleData>& rectangles)
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

        window.clear(sf::Color::Black);
        UpdateVisualisation(&window, rectangles);
        window.display();
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
void insertion_sort(std::vector<T>& input, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        cursor_positions.FirstCursor = i;
        for (int a = i; a > 0; a--)
        {
            cursor_positions.SecondCursor = a;
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
    timer.stop();
}

template <typename T>
void shell_sort(std::vector<T>& input, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    int h = 1;
    while (h < size / 3)
        h = 3 * h + 1;

    while (h >= 1)
    {
        for (int i = h; i < size; i++)
        {
            cursor_positions.FirstCursor = i;
            for (int a = i; a >= h; a -= h)
            {
                cursor_positions.SecondCursor = a;
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

    timer.stop();
}

template <typename T>
void selection_sort(std::vector<T>& input, bool (*greater_than)(T a, T b), int delay = 0)
{
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        cursor_positions.FirstCursor = i;
        size_t smallest_value = i;
        for (int a = i + 1; a < size; a++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            cursor_positions.SecondCursor = a;
            comparisons++;
            if (greater_than(input[smallest_value], input[a]))
                smallest_value = a;
        }
        swap<T>(input, i, smallest_value);
    }
    timer.stop();
}

template <typename T>
void merge(
    std::vector<T>& input, std::vector<T>& aux, int lo, int mid, int hi, bool (*higher_than)(T a, T b), int delay)
{
    for (int i = lo; i <= hi; i++)
        aux[i] = input[i];

    int i = lo, j = mid + 1;
    for (int k = lo; k <= hi; k++)
    {
        cursor_positions.FirstCursor = mid;
        cursor_positions.SecondCursor = k;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (i > mid)
        {
            input[k] = aux[j];
            ++j;
        }
        else if (j > hi)
        {
            input[k] = aux[i];
            ++i;
        }
        else if (!higher_than(aux[j], aux[i]))
        {
            ++comparisons;
            ++swaps;
            input[k] = aux[j];
            ++j;
        }
        else
        {
            ++comparisons;
            ++swaps;
            input[k] = aux[i];
            ++i;
        }
    }
}

template <typename T>
void merge_sort_recrussion(
    std::vector<T>& input, std::vector<T>& aux, int lo, int hi, bool (*higher_than)(T a, T b), int delay)
{
    if (hi <= lo)
        return;

    int mid = lo + (hi - lo) / 2;

    merge_sort_recrussion<T>(input, aux, lo, mid, higher_than, delay);
    merge_sort_recrussion<T>(input, aux, mid + 1, hi, higher_than, delay);
    merge<T>(input, aux, lo, mid, hi, higher_than, delay);
}

template <typename T>
void merge_sort(std::vector<T>& input, bool (*higher_than)(T a, T b), int delay)
{
    std::vector<T> aux;
    aux.reserve(input.size());
    for (T item : input)
        aux.push_back(item);

    merge_sort_recrussion<T>(input, aux, 0, input.size() - 1, higher_than, delay);
    timer.stop();
}

template <typename T>
void botton_up_merge_sort(std::vector<T>& input, bool (*higher_than)(T a, T b), int delay)
{
    std::vector<T> aux;
    aux.reserve(input.size());
    for (T item : input)
        aux.push_back(item);

    for (size_t i = 1; i < input.size(); i = i + i)
        for (size_t a = 0; a < input.size() - i; a += i + i)
            merge(
                input, aux, a, a + i - 1, std::min(a + i + i - 1, input.size() - 1), higher_than,
                delay);

    timer.stop();
}

template <typename T>
int partition(std::vector<T>& input, int lo, int hi, bool (*higher_than)(T a, T b), int delay)
{
    int i = lo, j = hi + 1;

    while (true)
    {
        while (!higher_than(input[++i], input[lo]))
        {
            ++comparisons;
            if (i == hi)
                break;
        }
           

        while (!higher_than(input[lo], input[--j]))
        {
            ++comparisons;
            if (j == lo)
                break;
        }
            

          if (i >= j)
              break;

          std::this_thread::sleep_for(std::chrono::milliseconds(delay));
          cursor_positions.SecondCursor = j;
          cursor_positions.FirstCursor = i;
          swap(input, i, j);
    }

    swap(input, lo, j);
    return j;
}

template<typename T>
void quick_sort_recrussion(std::vector<T>& input, int lo, int hi, bool (*higher_than)(T a, T b), int delay)
{
    if (hi <= lo)
        return;

    int j = partition<T>(input, lo, hi, higher_than, delay);
    quick_sort_recrussion<T>(input, lo, j - 1, higher_than, delay);
    quick_sort_recrussion<T>(input, j + 1, hi, higher_than, delay);
}

template <typename T>
void quick_sort(std::vector<T>& input, bool (*higher_than)(T a, T b), int delay)
{
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(input), std::end(input), rng);
    quick_sort_recrussion<T>(input, 0, input.size() - 1, higher_than, delay);
    timer.stop();
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
    std::cout << "MergeSort" << std::endl;
    std::cout << "BottonUpMergeSort" << std::endl;
    std::cout << "QuickSort" << std::endl;

    std::string type;
    while (true)
    {
        std::cin >> type;

        if (type == "SelectionSort" || type == "InsertionSort" || type == "ShellSort" || type == "MergeSort" ||
            type == "BottonUpMergeSort" || type == "QuickSort")
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

void CreateText(std::string sort, size_t items)
{
    font.loadFromFile("Roboto.ttf");

    sort_text.setFont(font);
    sort_text.setCharacterSize(15);
    sort_text.setString(sort);

    items_text.setFont(font);
    items_text.setCharacterSize(15);
    items_text.setPosition(0, 20);
    items_text.setString("Items: " + std::to_string(items));

    comparsion_text.setFont(font);
    comparsion_text.setCharacterSize(15);
    comparsion_text.setPosition(0, 40);

    swap_text.setFont(font);
    swap_text.setCharacterSize(15);
    swap_text.setPosition(0, 60);

    timer_text.setFont(font);
    timer_text.setCharacterSize(15);
    timer_text.setPosition(0, 80);

    timer.start();
}

int main()
{

    InputData input = inputs();
    CreateText(input.sort, input.sizes.size());
    std::vector<RectangleData> rectangles = create_rectangles(input.sizes, 1900.0f / input.sizes.size());

    auto greater_than = [](RectangleData a, RectangleData b) { return a.m_size > b.m_size; };

    std::thread thread;
    if (input.sort == "SelectionSort")
        thread = std::thread(selection_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);
    else if (input.sort == "InsertionSort")
        thread = std::thread(insertion_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);
    else if (input.sort == "ShellSort")
        thread = std::thread(shell_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);
    else if (input.sort == "MergeSort")
        thread = std::thread(merge_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);
    else if (input.sort == "BottonUpMergeSort")
        thread = std::thread(botton_up_merge_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);
    else if (input.sort == "QuickSort")
        thread = std::thread(quick_sort<RectangleData>, std::ref(rectangles), greater_than, input.delay);

    Render(rectangles);
}

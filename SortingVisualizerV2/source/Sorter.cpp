#include "Sorter.h"
#include <random>

sorter::sorter() : m_current_sort(sorts::selection_sort), m_max_size(75), m_delay(1), m_is_sorting(false)
{
    m_items.reserve(20);
    for (unsigned int i = 0; i < 20; i++)
    {
        m_items.push_back((rand() % m_max_size) + 1);
    }
}

sorter::~sorter()
{
    m_is_sorting = false;
    m_sorting._Is_ready();
}

void sorter::set_size(unsigned int size)
{
    if (m_is_sorting)
        return;

    std::vector<int> temp;
    temp.reserve(size);

    for (unsigned int i = 0; i < size; i++)
    {
        temp.push_back((rand() % m_max_size) + 1);
    }
    m_items = std::move(temp);
}

void sorter::set_sort(sorts sort)
{
    if (m_is_sorting)
        return;

    m_current_sort = sort;
}

void sorter::randomize()
{
    if (m_is_sorting)
        return;

    for (int& item : m_items)
    {
        item = ((rand() % m_max_size) + 1);
    }
}

void sorter::start_sort()
{
    if (m_is_sorting)
        return;

    auto greater_than = [](int a, int b) { return a > b; };

    m_output = sorting_output();
    m_is_sorting = true;
    switch (m_current_sort)
    {
    case sorts::selection_sort:
        m_sorting = std::async(
            std::launch::async, selection_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;

    case sorts::insertion_sort:
        m_sorting = std::async(
            std::launch::async, insertion_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;

    case sorts::shell_sort:
        m_sorting = std::async(
            std::launch::async, shell_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;

    case sorts::merge_sort:
        m_sorting = std::async(
            std::launch::async, merge_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;

    case sorts::botton_up_merge_sort:
        m_sorting = std::async(
            std::launch::async, botton_up_merge_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting),
            m_delay, greater_than);
        break;

    case sorts::quick_sort:
        m_sorting = std::async(
            std::launch::async, quick_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;

    case sorts::heap_sort:
        m_sorting = std::async(
            std::launch::async, heap_sort, std::ref(m_items), std::ref(m_output), std::ref(m_is_sorting), m_delay,
            greater_than);
        break;
    }
}

void sorter::stop_sort() { m_is_sorting = false; }

void sorter::swap(std::vector<int>& input, size_t index_1, size_t index_2)
{
    int temp_value = input[index_1];
    input[index_1] = input[index_2];
    input[index_2] = temp_value;
}

void sorter::selection_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    size_t size = input.size();
    for (int i = 0; i < size; i++)
    {
        output.first_cursor_position = i;
        size_t smallest_value = i;
        for (int a = i + 1; a < size; a++)
        {
            if (!on_going)
                return;

            output.second_cursor_position = a;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            ++output.comparisons;
            if (greater_than(input[smallest_value], input[a]))
                smallest_value = a;
        }
        ++output.swaps;
        swap(input, i, smallest_value);
    }

    on_going = false;
}

void sorter::insertion_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    int size = input.size();
    for (int i = 0; i < size; i++)
    {
        output.first_cursor_position = i;
        for (int a = i; a > 0; a--)
        {
            if (!on_going)
                return;

            output.second_cursor_position = a;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            ++output.comparisons;
            if (!greater_than(input[a], input[a - 1]))
            {
                ++output.swaps;
                swap(input, a, a - 1);
            }
            else
                break;
        }
    }
    on_going = false;
}

void sorter::shell_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    int size = input.size();
    int h = 1;
    while (h < size / 3)
        h = 3 * h + 1;

    while (h >= 1)
    {
        for (int i = h; i < size; i++)
        {
            output.first_cursor_position = i;
            for (int a = i; a >= h; a -= h)
            {
                if (!on_going)
                    return;

                output.second_cursor_position = a;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                ++output.comparisons;
                if (!greater_than(input[a], input[a - h]))
                {
                    ++output.swaps;
                    swap(input, a, a - h);
                }
                else
                    break;
            }
        }
        h = h / 3;
    }

    on_going = false;
}

void sorter::merge(
    std::vector<int>& input, std::vector<int>& aux, sorting_output& output, bool& on_going, int lo, int mid, int hi,
    bool (*higher_than)(int a, int b), int delay)
{
    for (int i = lo; i <= hi; i++)
        aux[i] = input[i];

    int i = lo, j = mid + 1;
    for (int k = lo; k <= hi; k++)
    {

        if (!on_going)
            return;

        output.first_cursor_position = mid;
        output.second_cursor_position = k;
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
            ++output.comparisons;
            ++output.swaps;
            input[k] = aux[j];
            ++j;
        }
        else
        {
            ++output.comparisons;
            ++output.swaps;
            input[k] = aux[i];
            ++i;
        }
    }
}

void sorter::merge_sort_recrussion(
    std::vector<int>& input, std::vector<int>& aux, sorting_output& output, bool& on_going, int lo, int hi,
    bool (*higher_than)(int a, int b), int delay)
{
    if (hi <= lo)
        return;

    if (!on_going)
        return;

    int mid = lo + (hi - lo) / 2;

    merge_sort_recrussion(input, aux, output, on_going, lo, mid, higher_than, delay);
    merge_sort_recrussion(input, aux, output, on_going, mid + 1, hi, higher_than, delay);
    merge(input, aux, output, on_going, lo, mid, hi, higher_than, delay);
}

void sorter::merge_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    std::vector<int> aux;
    aux.reserve(input.size());
    for (int item : input)
        aux.push_back(item);

    merge_sort_recrussion(input, aux, output, on_going, 0, input.size() - 1, greater_than, delay);
    on_going = false;
}

void sorter::botton_up_merge_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    std::vector<int> aux;
    aux.reserve(input.size());
    for (int item : input)
        aux.push_back(item);

    for (size_t i = 1; i < input.size(); i = i + i)
        for (size_t a = 0; a < input.size() - i; a += i + i)
        {
            if (!on_going)
                return;
            merge(
                input, aux, output, on_going, a, a + i - 1, std::min(a + i + i - 1, input.size() - 1), greater_than,
                delay);
        }
    on_going = false;
}

int sorter::partition(
    std::vector<int>& input, sorting_output& output, bool& on_going, int lo, int hi, bool (*higher_than)(int a, int b),
    int delay)
{
    int i = lo, j = hi + 1;

    while (true)
    {
        while (!higher_than(input[++i], input[lo]))
        {
            if (!on_going)
                return 0;

            ++output.comparisons;
            if (i == hi)
                break;
        }

        while (!higher_than(input[lo], input[--j]))
        {
            if (!on_going)
                return 0;

            ++output.comparisons;
            if (j == lo)
                break;
        }

        if (i >= j)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        output.second_cursor_position = j;
        output.first_cursor_position = i;
        ++output.swaps;
        swap(input, i, j);
    }

    swap(input, lo, j);
    return j;
}

void sorter::heap_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    int size = input.size();
    output.first_cursor_position = size - 1;
    for (int i = (size / 2); i >= 0; i--)
    {
        if (!on_going)
            return;
        sink(input, output, on_going, delay, i, size, greater_than);
    }

    while (size > 1)
    {
        if (!on_going)
            return;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        ++output.swaps;
        output.first_cursor_position = size - 1;
        swap(input, 0, size - 1);
        sink(input, output, on_going, delay, 0, --size, greater_than);
    }

    on_going = false;
}

void sorter::sink(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, int index, int size,
    bool (*greater_than)(int a, int b))
{
    while ((index * 2) <= size-1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        int j = 2 * index;

        output.second_cursor_position = index;

        ++output.comparisons;
        if (j < size - 1 && !greater_than(input[j], input[j + 1]))
            ++j;

        ++output.comparisons;
        if (greater_than(input[index], input[j]) || input[index] == input[j])
            break;

        if (!on_going)
            return;

        ++output.swaps;
        swap(input, index, j);
        index = j;
    }
}

void sorter::quick_sort_recrussion(
    std::vector<int>& input, sorting_output& output, bool& on_going, int lo, int hi, bool (*greater_than)(int a, int b),
    int delay)
{
    if (hi <= lo)
        return;

    if (!on_going)
        return;

    int j = partition(input, output, on_going, lo, hi, greater_than, delay);
    quick_sort_recrussion(input, output, on_going, lo, j - 1, greater_than, delay);
    quick_sort_recrussion(input, output, on_going, j + 1, hi, greater_than, delay);
}

void sorter::quick_sort(
    std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b))
{
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(input), std::end(input), rng);
    quick_sort_recrussion(input, output, on_going, 0, input.size() - 1, greater_than, delay);

    on_going = false;
}

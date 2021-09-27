#pragma once

#include <future>
#include <vector>

enum class sorts
{
    selection_sort = 0,
    insertion_sort,
    shell_sort,
    merge_sort,
    botton_up_merge_sort,
    quick_sort
};

struct sorting_output
{
    sorting_output() : first_cursor_position(0), second_cursor_position(0), swaps(0), comparisons(0) {}

    int first_cursor_position;
    int second_cursor_position;
    int swaps;
    int comparisons;
};

class sorter
{
public:
    sorter();

    void set_size(unsigned int size);
    void set_sort(sorts sort);
    void randomize();

    void start_sort();
    void stop_sort();

    inline void set_max_size(unsigned int new_size) { m_max_size = new_size; }
    inline void set_delay(unsigned int new_delay) { m_delay = new_delay; };

    inline size_t get_size() const { return m_items.size(); }
    inline const std::vector<int>& get_items() const { return m_items; }
    inline const sorting_output& get_output() const { return m_output; }
    inline unsigned int get_delay() const { return m_delay; };
    inline bool is_sorting() const { return m_is_sorting; }

private:
    static void swap(std::vector<int>& input, size_t index_1, size_t index_2);
    static void selection_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void insertion_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void shell_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void merge_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void merge_sort_recrussion(
        std::vector<int>& input, std::vector<int>& aux, sorting_output& output, bool& on_going, int lo, int hi,
        bool (*higher_than)(int a, int b), int delay);
    static void merge(
        std::vector<int>& input, std::vector<int>& aux, sorting_output& output, bool& on_going, int lo, int mid, int hi,
        bool (*higher_than)(int a, int b), int delay);
    static void botton_up_merge_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void quick_sort(
        std::vector<int>& input, sorting_output& output, bool& on_going, int delay, bool (*greater_than)(int a, int b));
    static void quick_sort_recrussion(
        std::vector<int>& input, sorting_output& output, bool& on_going, int lo, int hi,
        bool (*higher_than)(int a, int b), int delay);
    static int partition(
        std::vector<int>& input, sorting_output& output, bool& on_going, int lo, int hi,
        bool (*higher_than)(int a, int b), int delay);

    std::vector<int> m_items;
    sorts m_current_sort;
    unsigned int m_max_size;
    unsigned int m_delay;
    bool m_is_sorting;
    std::future<void> m_sorting;
    sorting_output m_output;
};

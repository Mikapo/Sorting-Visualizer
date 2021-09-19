#pragma once

#include <vector>
#include <thread>

namespace mikasort
{
    template <typename T>
    void merge(std::vector<T>& input, std::vector<T>& aux, int lo, int mid, int hi, bool (*higher_than)(T a, T b))
    {
        for (int i = lo; i <= hi; i++)
            aux[i] = input[i];

        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++)
        {
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
                input[k] = aux[j];
                ++j;
            }
            else
            {
                input[k] = aux[i];
                ++i;
            }
        }
    }

    template <typename T>
    void sort(std::vector<T>& input, std::vector<T>& aux, int lo, int hi, bool (*higher_than)(T a, T b), int delay)
    {
        if (hi <= lo)
            return;

        int mid = lo + (hi - lo) / 2;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        sort<T>(input, aux, lo, mid, higher_than, delay);
        sort<T>(input, aux, mid + 1, hi, higher_than, delay);
        merge<T>(input, aux, lo, mid, hi, higher_than);
    }

    template <typename T>
    void merge_sort(std::vector<T>& input, bool (*higher_than)(T a, T b), int delay)
    {
        std::vector<T> aux;
        aux.reserve(input.size());
        for (T item : input)
            aux.push_back(item);

        sort<T>(input, aux, 0, input.size() - 1, higher_than, delay);

        
    }
} // namespace mikasort

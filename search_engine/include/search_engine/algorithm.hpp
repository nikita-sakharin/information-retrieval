#ifndef SEARCH_ENGINE_ALGORITHM_HPP
#define SEARCH_ENGINE_ALGORITHM_HPP

#include <cstddef>

#include <functional>
#include <iterator>
#include <utility>

template<class ForwardIter, class T, class Compare>
ForwardIter binary_search(ForwardIter first, ForwardIter last,
    const T &value, Compare comp)
{
    std::ptrdiff_t count = std::distance(first, last);
    for (ForwardIter iter; iter = first, count > 0; )
    {
        const std::ptrdiff_t step = count / 2;
        std::advance(iter, step);
        if (comp(*iter, value))
        {
            first = ++iter;
            count -= step + 1;
        }
        else if (comp(value, *iter))
            count = step;
        else
            return iter;
    }
    return last;
}

template<class ForwardIter, class T>
ForwardIter binary_search(ForwardIter first, ForwardIter last,
    const T &value)
{
    return binary_search(first, last, value, std::less<T>());
}

template<class RandomAccessIter, class Compare>
void pop_heap(
    const RandomAccessIter first,
    RandomAccessIter last,
    const Compare comp
) {
    std::swap(*first, *--last);
    const std::ptrdiff_t half = (last - first) / 2;
    for (RandomAccessIter parent = first, child = first;
        parent - first < half; parent = child)
    {
        child += parent - first + 1;
        if (child + 1 != last && comp(*child, *(child + 1)))
            ++child;
        if (!comp(*parent, *child))
            break;
        std::swap(*child, *parent);
    }
}

template<class RandomAccessIter, class Compare>
void push_heap(
    const RandomAccessIter first,
    RandomAccessIter last,
    const Compare comp
) {
    --last;
    for (RandomAccessIter parent; first != last; last = parent) {
        parent = first + (last - first - 1) / 2;
        if (!comp(*parent, *last))
            break;
        std::swap(*parent, *last);
    }
}

template<class RandomAccessIter, class Compare>
void make_heap(
    const RandomAccessIter first,
    const RandomAccessIter last,
    const Compare comp
) {
    for (RandomAccessIter current = first; current != last; )
        push_heap(first, ++current, comp);
}

template<class RandomAccessIter, class Compare>
void sort_heap(
    const RandomAccessIter first,
    const RandomAccessIter last,
    const Compare comp
) {
    for (RandomAccessIter current = last; current != first; --current)
        pop_heap(first, current, comp);
}

template<class RandomAccessIter, class Compare>
void sort(
    const RandomAccessIter first,
    const RandomAccessIter last,
    const Compare comp
) {
    make_heap(first, last, comp);
    sort_heap(first, last, comp);
}

#endif

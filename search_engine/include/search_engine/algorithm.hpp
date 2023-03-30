#ifndef SEARCH_ENGINE_ALGORITHM_HPP
#define SEARCH_ENGINE_ALGORITHM_HPP

#include <cstddef> // ptrdiff_t

#include <functional> // less
#include <iterator>
#include <utility> // swap

template<class ForwardIter, class T, class Compare>
ForwardIter binary_search(
    ForwardIter first,
    ForwardIter last,
    const T &value,
    const Compare comp
) {
    using std::ptrdiff_t;

    ptrdiff_t count = std::distance(first, last);
    for (ForwardIter iter; iter = first, count > 0; ) {
        const ptrdiff_t step = count / 2;
        std::advance(iter, step);
        if (comp(*iter, value)) {
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
ForwardIter binary_search(ForwardIter first, ForwardIter last, const T &value) {
    using std::less;

    return binary_search(first, last, value, less<T>());
}

template<class RandomAccessIter, class Compare>
void pop_heap(
    const RandomAccessIter first,
    RandomAccessIter last,
    const Compare comp
) {
    using std::ptrdiff_t, std::swap;

    swap(*first, *--last);
    const ptrdiff_t half = (last - first) / 2;
    for (RandomAccessIter parent = first, child = first;
        parent - first < half; parent = child
    ) {
        child += parent - first + 1;
        if (child + 1 != last && comp(*child, *(child + 1)))
            ++child;
        if (!comp(*parent, *child))
            break;
        swap(*child, *parent);
    }
}

template<class RandomAccessIter, class Compare>
void push_heap(
    const RandomAccessIter first,
    RandomAccessIter last,
    const Compare comp
) {
    using std::swap;

    --last;
    for (RandomAccessIter parent; first != last; last = parent) {
        parent = first + (last - first - 1) / 2;
        if (!comp(*parent, *last))
            break;
        swap(*parent, *last);
    }
}

template<class RandomAccessIter, class Compare>
void make_heap(
    const RandomAccessIter first,
    const RandomAccessIter last,
    const Compare comp
) {
    for (RandomAccessIter current = first; current != last; ++current)
        push_heap(first, current, comp);
}

template<class RandomAccessIter, class Compare>
void sort_heap(
    const RandomAccessIter first,
    const RandomAccessIter last,
    const Compare comp
) {
    for (; last != first; --last)
        pop_heap(first, last, comp);
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

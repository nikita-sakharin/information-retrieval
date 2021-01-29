#ifndef SEARCH_ENGINE_MEMORY_HPP
#define SEARCH_ENGINE_MEMORY_HPP

#include <cstddef> // size_t

#include <limits> // numeric_limits
#include <type_traits> // is_integral_v

static constexpr std::size_t KiB4 = 1U << 12,
    MiB2 = 1UL << 21,
    GiB1 = 1UL << 30;

template<typename T>
static constexpr T pow2round(T value) noexcept {
    using std::is_integral_v, std::numeric_limits, std::size_t;
    static_assert(is_integral_v<T>, "integral type required");

    --value;
    constexpr size_t size = sizeof(size_t) * numeric_limits<uchar>::digits;
    for (size_t i = 1U; i < size; i <<= 1)
        value |= value >> i;
    return ++value;
}

#endif

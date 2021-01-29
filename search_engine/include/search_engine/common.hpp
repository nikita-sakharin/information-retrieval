#ifndef SEARCH_ENGINE_COMMON_HPP
#define SEARCH_ENGINE_COMMON_HPP

#include <cstddef> // size_t

#include <limits> // numeric_limits
#include <type_traits> // is_integral_v

using schar  = signed char;
using uchar  = unsigned char;
using shrt   = short int;
using ushrt  = unsigned short int;
using uint   = unsigned int;
using ulong  = unsigned long int;
using llong  = long long int;
using ullong = unsigned long long int;
using flt    = float;
using dbl    = double;
using ldbl   = long double;

constexpr std::size_t KiB4 = 1U << 12, MiB2 = 1UL << 21, GiB1 = 1UL << 30;

template<typename T>
constexpr T pow2round(T value) noexcept {
    using std::is_integral_v, std::numeric_limits, std::size_t;
    static_assert(is_integral_v<T>, "integral type required");

    --value;
    constexpr size_t size = sizeof(T) * numeric_limits<uchar>::digits;
    for (size_t i = 1U; i < size; i <<= 1)
        value |= value >> i;
    return ++value;
}

#endif
/*
    assert(pow2round<ptrdiff_t>(0) == 0);
    assert(pow2round<ptrdiff_t>(1) == 1);
    assert(pow2round<ptrdiff_t>(2) == 2);
    assert(pow2round<ptrdiff_t>(3) == 4);
    assert(pow2round<ptrdiff_t>(4) == 4);
    assert(pow2round<ptrdiff_t>(5) == 8);
    assert(pow2round<ptrdiff_t>(6) == 8);
    assert(pow2round<ptrdiff_t>(7) == 8);
    assert(pow2round<ptrdiff_t>(8) == 8);
    assert(pow2round<ptrdiff_t>(9) == 16);
    assert(pow2round<ptrdiff_t>(10) == 16);
    assert(pow2round<ptrdiff_t>(11) == 16);
    assert(pow2round<ptrdiff_t>(12) == 16);
    assert(pow2round<ptrdiff_t>(13) == 16);
    assert(pow2round<ptrdiff_t>(14) == 16);
    assert(pow2round<ptrdiff_t>(15) == 16);
    assert(pow2round<ptrdiff_t>(16) == 16);
    assert(pow2round<ptrdiff_t>(17) == 32);
    assert(pow2round<ptrdiff_t>(18) == 32);
    assert(pow2round<ptrdiff_t>(19) == 32);
    assert(pow2round<ptrdiff_t>(20) == 32);
    assert(pow2round<ptrdiff_t>(21) == 32);
    assert(pow2round<ptrdiff_t>(22) == 32);
    assert(pow2round<ptrdiff_t>(23) == 32);
    assert(pow2round<ptrdiff_t>(24) == 32);
    assert(pow2round<ptrdiff_t>(25) == 32);
    assert(pow2round<ptrdiff_t>(26) == 32);
    assert(pow2round<ptrdiff_t>(27) == 32);
    assert(pow2round<ptrdiff_t>(28) == 32);
    assert(pow2round<ptrdiff_t>(29) == 32);
    assert(pow2round<ptrdiff_t>(30) == 32);
    assert(pow2round<ptrdiff_t>(31) == 32);
    assert(pow2round<ptrdiff_t>(32) == 32);
    assert(pow2round<ptrdiff_t>((1LL << 31) - 3) == 1LL << 31);
    assert(pow2round<ptrdiff_t>((1LL << 31) - 2) == 1LL << 31);
    assert(pow2round<ptrdiff_t>((1LL << 31) - 1) == 1LL << 31);
    assert(pow2round<ptrdiff_t>((1LL << 31) + 0) == 1LL << 31);
    assert(pow2round<ptrdiff_t>((1LL << 31) + 1) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 31) + 2) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 31) + 3) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 32) - 3) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 32) - 2) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 32) - 1) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 32) + 0) == 1LL << 32);
    assert(pow2round<ptrdiff_t>((1LL << 32) + 1) == 1LL << 33);
    assert(pow2round<ptrdiff_t>((1LL << 32) + 2) == 1LL << 33);
    assert(pow2round<ptrdiff_t>((1LL << 32) + 3) == 1LL << 33);
    assert(pow2round<ptrdiff_t>((1LL << 62) - 3) == 1LL << 62);
    assert(pow2round<ptrdiff_t>((1LL << 62) - 2) == 1LL << 62);
    assert(pow2round<ptrdiff_t>((1LL << 62) - 1) == 1LL << 63);
    // assert(pow2round<ptrdiff_t>((1LL << 63) + 0) == 1LL << 63);

    assert(pow2round<size_t>(0) == 0);
    assert(pow2round<size_t>(1) == 1);
    assert(pow2round<size_t>(2) == 2);
    assert(pow2round<size_t>(3) == 4);
    assert(pow2round<size_t>(4) == 4);
    assert(pow2round<size_t>(5) == 8);
    assert(pow2round<size_t>(6) == 8);
    assert(pow2round<size_t>(7) == 8);
    assert(pow2round<size_t>(8) == 8);
    assert(pow2round<size_t>(9) == 16);
    assert(pow2round<size_t>(10) == 16);
    assert(pow2round<size_t>(11) == 16);
    assert(pow2round<size_t>(12) == 16);
    assert(pow2round<size_t>(13) == 16);
    assert(pow2round<size_t>(14) == 16);
    assert(pow2round<size_t>(15) == 16);
    assert(pow2round<size_t>(16) == 16);
    assert(pow2round<size_t>(17) == 32);
    assert(pow2round<size_t>(18) == 32);
    assert(pow2round<size_t>(19) == 32);
    assert(pow2round<size_t>(20) == 32);
    assert(pow2round<size_t>(21) == 32);
    assert(pow2round<size_t>(22) == 32);
    assert(pow2round<size_t>(23) == 32);
    assert(pow2round<size_t>(24) == 32);
    assert(pow2round<size_t>(25) == 32);
    assert(pow2round<size_t>(26) == 32);
    assert(pow2round<size_t>(27) == 32);
    assert(pow2round<size_t>(28) == 32);
    assert(pow2round<size_t>(29) == 32);
    assert(pow2round<size_t>(30) == 32);
    assert(pow2round<size_t>(31) == 32);
    assert(pow2round<size_t>(32) == 32);
    assert(pow2round<size_t>((1ULL << 31) - 3) == 1ULL << 31);
    assert(pow2round<size_t>((1ULL << 31) - 2) == 1ULL << 31);
    assert(pow2round<size_t>((1ULL << 31) - 1) == 1ULL << 31);
    assert(pow2round<size_t>((1ULL << 31) + 0) == 1ULL << 31);
    assert(pow2round<size_t>((1ULL << 31) + 1) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 31) + 2) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 31) + 3) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 32) - 3) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 32) - 2) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 32) - 1) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 32) + 0) == 1ULL << 32);
    assert(pow2round<size_t>((1ULL << 32) + 1) == 1ULL << 33);
    assert(pow2round<size_t>((1ULL << 32) + 2) == 1ULL << 33);
    assert(pow2round<size_t>((1ULL << 32) + 3) == 1ULL << 33);
    assert(pow2round<size_t>((1ULL << 63) - 3) == 1ULL << 63);
    assert(pow2round<size_t>((1ULL << 63) - 2) == 1ULL << 63);
    assert(pow2round<size_t>((1ULL << 63) - 1) == 1ULL << 63);
    assert(pow2round<size_t>((1ULL << 63) + 0) == 1ULL << 63);
*/

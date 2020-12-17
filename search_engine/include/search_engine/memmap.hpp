#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

#include <cstddef>

#include <sys/types.h>

class memmap final {
    class file final {
        int fildes_;

    public:
        inline constexpr file() noexcept;
        inline file(const char *);
        inline constexpr file(const file &) noexcept = delete;
        inline constexpr file(file &&);
        inline constexpr file &operator=(const file &) noexcept = delete;
        inline constexpr file &operator=(file &&);
        inline ~file() noexcept;

        inline void close();
        inline constexpr int fildes() const noexcept;
        inline constexpr bool is_open() const noexcept;
        inline void open(const char *);
        inline off_t size() const;
    };

    static constexpr std::size_t max_len = 1ULL << 32;

    off_t off_;
    off_t size_;
    const void *addr_;
    file file_;

public:
    inline memmap() noexcept;
    inline memmap(const char *);
    inline constexpr memmap(const memmap &) noexcept = delete;
    inline memmap(memmap &&);
    inline constexpr memmap &operator=(const memmap &) noexcept = delete;
    memmap &operator=(memmap &&);
    inline ~memmap() noexcept;

    void close();
    inline const char *data() const noexcept;
    inline bool is_open() const noexcept;
    inline size_t length() const noexcept;
    inline void open(const char *);
    void seek(off_t);
    inline off_t size() const noexcept;
    inline off_t tell() const noexcept;
};

#endif

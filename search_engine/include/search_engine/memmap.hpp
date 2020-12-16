#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

#include <sys/types.h>

class memmap final {
    off_t off_;
    off_t size_;
    const void *addr_;
    int fildes_;

    static constexpr size_t max_len = 1ULL << 31;

public:
    memmap() noexcept;
    memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    memmap(memmap &&);
    constexpr memmap &operator=(const memmap &) noexcept = delete;
    memmap &operator=(memmap &&);
    ~memmap() noexcept;

    void close();
    const char *data() const noexcept;
    bool is_open() const noexcept;
    size_t length() const noexcept;
    void open(const char *);
    void seek(off_t);
    off_t size() const noexcept;
    off_t tell() const noexcept;
};

#endif

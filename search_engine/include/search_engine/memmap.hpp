#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

#include <sys/types.h>

class memmap final {
    off_t off_;
    off_t size_;
    const void *addr_;
    int fildes_;

    static constexpr size_t max_len = 1ULL << 32;

    inline void try_close() noexcept;

public:
    inline memmap() noexcept;
    inline memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    inline memmap(memmap &&);
    constexpr memmap &operator=(const memmap &) noexcept = delete;
    memmap &operator=(memmap &&);
    inline ~memmap() noexcept;

    void close();
    inline const char *data() const noexcept;
    inline bool is_open() const noexcept;
    inline size_t length() const noexcept;
    void open(const char *);
    void seek(off_t);
    inline off_t size() const noexcept;
    inline off_t tell() const noexcept;
};

#endif

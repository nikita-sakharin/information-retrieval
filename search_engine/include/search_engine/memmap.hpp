#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

class memmap final {
    off_t off;
    off_t len;
    const void *addr;
    int fildes;

    static constexpr size_t max_len = 1ULL << 31;
public:

    constexpr memmap() noexcept;
    memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    constexpr memmap(memmap &&) noexcept;
    memmap &operator=(const memmap &) noexcept = delete;
    constexpr memmap &operator=(memmap &&) noexcept;
    ~memmap() noexcept;

    void close() noexcept;
    const char *data() const noexcept;
    bool is_open() const noexcept;
    void open(const char *);
    size_t size() const noexcept;
};

#endif

#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

#include <cstddef> // size_t

class memmap final {
public:
    memmap() noexcept;
    explicit memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    memmap(memmap &&);
    constexpr memmap &operator=(const memmap &) noexcept = delete;
    memmap &operator=(memmap &&);
    ~memmap() noexcept;

    void close();
    const char *data() const;
    bool empty() const;
    bool is_open() const noexcept;
    void open(const char *);
    std::size_t size() const;
    void swap(memmap &) noexcept;

private:
    class file final {
    public:
        constexpr file() noexcept;
        inline explicit file(const char *);
        constexpr file(const file &) noexcept = delete;
        constexpr file(file &&);
        constexpr file &operator=(const file &) noexcept = delete;
        constexpr file &operator=(file &&);
        ~file() noexcept;

        void close();
        constexpr int fildes() const;
        constexpr bool is_open() const noexcept;
        void open(const char *);
        std::size_t size() const;
        constexpr void swap(file &) noexcept;

    private:
        int fildes_;
    };

    const void *addr_;
    std::size_t size_;
    file file_;
};

#endif

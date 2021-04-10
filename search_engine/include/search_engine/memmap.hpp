#ifndef SEARCH_ENGINE_MEMMAP_HPP
#define SEARCH_ENGINE_MEMMAP_HPP

#include <cassert> // assert
#include <cstddef> // size_t

#include <exception> // exception
#include <iostream> // cerr, endl
#include <limits> // numeric_limits
#include <stdexcept> // logic_error
#include <string_view> // string_view
#include <utility> // move, swap

class memmap final {
public:
    memmap() noexcept;
    inline explicit memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    memmap(memmap &&);
    constexpr memmap &operator=(const memmap &) noexcept = delete;
    memmap &operator=(memmap &&);
    ~memmap() noexcept;

    void close();
    const char *data() const;
    constexpr bool empty() const;
    constexpr bool is_open() const noexcept;
    void open(const char *);
    inline explicit operator std::string_view() const;
    constexpr std::size_t size() const;
    constexpr void swap(memmap &) noexcept;

private:
    class file final {
    public:
        constexpr file() noexcept = default;
        inline explicit file(const char *);
        constexpr file(const file &) noexcept = delete;
        constexpr file(file &&);
        constexpr file &operator=(const file &) noexcept = delete;
        constexpr file &operator=(file &&);
        constexpr ~file() noexcept;

        void close();
        constexpr int fildes() const;
        constexpr bool is_open() const noexcept;
        void open(const char *);
        std::size_t size() const;
        constexpr void swap(file &) noexcept;

    private:
        int fildes_ = -1;
    };

    using size_limits = std::numeric_limits<size_t>;

    const void *addr_;
    std::size_t size_ = size_limits::max();
    file file_{};
};

inline memmap::memmap(const char * const filename) : memmap() {
    open(filename);
}

constexpr bool memmap::empty() const {
    return size() == 0;
}

constexpr bool memmap::is_open() const noexcept {
    return file_.is_open();
}

inline memmap::operator std::string_view() const {
    using std::string_view;

    return string_view(data(), size_);
}

constexpr size_t memmap::size() const {
    using std::logic_error;

    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::size: memory map is not open");
    return size_;
}

constexpr void memmap::swap(memmap &rhs) noexcept {
    using std::swap;

    swap(addr_, rhs.addr_);
    swap(size_, rhs.size_);
    file_.swap(rhs.file_);
}

inline memmap::file::file(const char * const filename) {
    open(filename);
}

constexpr memmap::file::file(file &&rhs) {
    using std::move;

    *this = move(rhs);
    assert(rhs.fildes_ == -1);
}

constexpr memmap::file &memmap::file::operator=(file &&rhs) {
    if (is_open())
        close();
    assert(fildes_ == -1);

    swap(rhs);
    return *this;
}

constexpr memmap::file::~file() noexcept {
    if (is_open())
        try {
            close();
        } catch (const std::exception &except) {
#           ifndef NDEBUG
            std::cerr << except.what() << std::endl;
#           endif
        }
    assert(fildes_ == -1);
}

constexpr int memmap::file::fildes() const {
    using std::logic_error;

    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::file::fildes: file is not open");
    assert(fildes_ >= 0);

    return fildes_;
}

constexpr bool memmap::file::is_open() const noexcept {
    return fildes_ >= 0;
}

constexpr void memmap::file::swap(file &rhs) noexcept {
    using std::swap;

    std::swap(fildes_, rhs.fildes_);
}

#endif

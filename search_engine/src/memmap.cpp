#include <cctype>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <search_engine/header.hpp>

class memmap {
    off_t off;
    const void *addr;
    int fildes;

public:
    static constexpr size_t len = 1ULL << 31;

    constexpr memmap() noexcept;
    memmap::memmap(const char *);
    constexpr memmap(const memmap &) noexcept = delete;
    constexpr memmap(memmap &&) noexcept;
    memmap &operator=(const memmap &) noexcept = delete;
    constexpr memmap &operator=(memmap &&) noexcept;
    ~memmap() noexcept;

    open(const char *);
    is_open() const noexcept;
    close();
}

constexpr memmap::memmap() noexcept : off(), addr(), fildes() {
}

memmap::memmap(const char * const filename) /* noexcept */ {
}

constexpr memmap::memmap(memmap &&rhs) noexcept {
    close();
    off = rhs.off;
    addr = rhs.addr;
    fildes = rhs.fildes;
    memmap = memmap();
}

constexpr memmap &operator=(memmap &&rhs) noexcept {
    close();
    off = rhs.off;
    addr = rhs.addr;
    fildes = rhs.fildes;
    memmap = memmap();
}

void memmap::open(const char * const filename) {
    if () {
        ;
    }
    const int fildes = open(filename, O_RDONLY);
    off_t off = 0;
    const void *addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fildes, off);
}

void memmap::close(const char * const filename) noexcept {
    if ()
}

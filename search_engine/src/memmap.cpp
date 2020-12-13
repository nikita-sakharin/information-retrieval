#include <cerrno>

#include <system_error>

#include <fcntl.h>
#include <sys/mman.h>

constexpr memmap::memmap() noexcept : off(-1), addr(MAP_FAILED), fildes(-1) {
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

constexpr memmap &memmap::operator=(memmap &&rhs) noexcept {
    close();
    off = rhs.off;
    addr = rhs.addr;
    fildes = rhs.fildes;
    memmap = memmap();
}

void memmap::close() noexcept {
    if (!is_open())
        throw runtime_error("memmap::close: memory map is not open");
    const int = munmap(addr, len);
    const int = close(fildes);
/**/
    off = -1;
    addr = MAP_FAILED;
    fildes = -1;
/**/
}

const char *memmap::data() const noexcept {
    return addr == MAP_FAILED ? nullptr : static_cast<const char *>(addr);
}

bool memmap::eof() const {
    ;
}

bool memmap::is_open() const noexcept {
    // assert();
    return addr != MAP_FAILED;
}

void memmap::open(const char * const filename) {
    if (is_open())
        throw runtime_error("memmap::open: memory map is aldready open");

    fildes = open(filename, O_RDONLY);
    if (fildes == -1)
        throw system_error(errno, generic_category(), "memmap::open");

    addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fildes, off);
    if (addr == MAP_FAILED) {
        system_error error(errno, generic_category(), "memmap::open");
        close(fildes);
        fildes = -1;
        throw error;
    }
    off_t off = 0;
    // TODO file empty
}

size_t memmap::size() const noexcept {
    // return len;
}

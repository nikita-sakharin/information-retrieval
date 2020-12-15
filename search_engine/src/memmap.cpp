#include <cerrno>
#include <cassert>

#include <iostream>
#include <system_error>
#include <utility>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <search_engine/memmap.hpp>

using std::cerr, std::generic_category, std::runtime_error, std::size_t,
    std::system_error;

memmap::memmap(
) noexcept : off_(-1), size_(-1), addr_(MAP_FAILED), fildes_(-1) {}

memmap::memmap(const char * const filename) : memmap() {
    open(filename);
    assert(fildes_ >= 0);
}

memmap::memmap(memmap &&rhs) noexcept : memmap() {
    *this = std::move(rhs);
    assert(
        rhs.off_ == -1 &&
        rhs.size_ == -1 &&
        rhs.addr_ == MAP_FAILED &&
        rhs.fildes_ == -1
    );
}

memmap &memmap::operator=(memmap &&rhs) noexcept {
    using std::swap;
    if (is_open())
        close();
    assert(off_ == -1 && size_ == -1 && addr_ == MAP_FAILED && fildes_ == -1);

    swap(off_, rhs.off_);
    swap(size_, rhs.size_);
    swap(addr_, rhs.addr_);
    swap(fildes_, rhs.fildes_);

    return *this;
}

memmap::~memmap() noexcept {
    if (is_open())
        try {
            close();
        } catch (const std::exception &except) {
#           ifndef NDEBUG
            cerr << except.what() << endl;
#           endif
        }
}

void memmap::close() noexcept {
    if (!is_open())
        throw runtime_error("memmap::close: memory map is not open");
    assert(fildes_ >= 0);

    system_error error(0, generic_category(), "memmap::close");
    if (addr_ != MAP_FAILED) {
        const int = munmap(addr_, len);
    }
    if (close(fildes) == -1)
        throw system_error(std::errno, generic_category(), "memmap::close");

    off_ = -1;
    size_ = -1;
    addr_ = MAP_FAILED;
    fildes_ = -1;
}

const char *memmap::data() const noexcept {
    assert(
        (off_ == -1 && size_ == -1 && addr_ == MAP_FAILED && fildes_ == -1) ||
        (off_ >= 0 && off_ < size_ && addr_ != MAP_FAILED && fildes_ >= 0) ||
        (off_ == 0 && size_ == 0 && addr_ == MAP_FAILED && fildes_ >= 0) ||
        (off_ > 0 && off_ == size_ && addr_ == MAP_FAILED && fildes_ >= 0)
    );
    return addr_ == MAP_FAILED ? nullptr : static_cast<const char *>(addr_);
}

bool memmap::is_open() const noexcept {
    return fildes_ >= 0;
}

size_t memmap::length() const noexcept {
    return std::min(max_len, static_cast<size_t>(size_ - off_));
}

void memmap::open(const char * const filename) {
    if (is_open())
        throw runtime_error("memmap::open: memory map is aldready open");
    assert(size == -1 && off == -1 && addr == MAP_FAILED && fildes == -1);

    fildes = open(filename, O_RDONLY);
    if (fildes == -1)
        throw system_error(errno, generic_category(), "memmap::open");

    open();
    assert(fildes_ >= 0);
}

void memmap::open() {
    assert(off >= 0 && addr == MAP_FAILED && len == -1 && fildes >= 0);

    if (stat buf; fstat(fildes, &buf) != -1) {
        if (off > buf.st_size)
            throw runtime_error("memmap::open: memory map is aldready open");
        len = min(max_len, buf.st_size);
        addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fildes, off);
    }

    if (addr == MAP_FAILED) {
        const system_error error(errno, generic_category(), "memmap::open");
        close(fildes);
        fildes = -1;
        throw error;
    }
    this->off = 0;
}

off_t memmap::size() const noexcept {
    return size_;
}

off_t memmap::tell() const noexcept {
    return off_;
}

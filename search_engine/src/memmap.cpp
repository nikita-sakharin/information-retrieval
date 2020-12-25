#include <cassert>
#include <cerrno>

#include <iostream> // cerr
#include <limits>
#include <system_error> // generic_category, system_error
#include <utility> // swap

#include <fcntl.h> // open
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // fstat
#include <unistd.h> // close

#include <search_engine/memmap.hpp>

using std::cerr, std::endl, std::generic_category, std::logic_error, std::move,
    std::size_t, std::swap, std::system_error;
using size_limits = std::numeric_limits<size_t>;

constexpr memmap::file::file() noexcept : fildes_(-1) {}

inline memmap::file::file(const char * const filename) : file() {
    open(filename);
    assert(fildes_ >= 0);
}

constexpr memmap::file::file(file &&rhs) : file() {
    *this = move(rhs);
    assert(rhs.fildes_ == -1);
}

constexpr memmap::file &memmap::file::operator=(file &&rhs) {
    if (is_open())
        close();
    assert(fildes_ == -1);

    swap(fildes_, rhs.fildes_);
    return *this;
}

inline memmap::file::~file() noexcept {
    if (is_open())
        try {
            close();
        } catch (const std::exception &except) {
#           ifndef NDEBUG
            cerr << except.what() << std::endl;
#           endif
        }
    assert(fildes_ == -1);
}

void memmap::file::close() {
    if (!is_open()) [[unlikely]]
        throw logic_error("file::close: file is not open");
    assert(fildes_ >= 0);

    const int returns = ::close(fildes_);
    fildes_ = -1;
    if (returns == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "file::close");
}

constexpr int memmap::file::fildes() const {
    if (!is_open()) [[unlikely]]
        throw logic_error("file::fildes: file is not open");
    assert(fildes_ >= 0);

    return fildes_;
}

constexpr bool memmap::file::is_open() const noexcept {
    return fildes_ >= 0;
}

void memmap::file::open(const char * const filename) {
    if (is_open()) [[unlikely]]
        throw logic_error("file::open: file is aldready open");
    assert(fildes_ == -1);

    fildes_ = ::open(filename, O_RDONLY);
    if (fildes_ == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "file::open");
    assert(fildes_ >= 0);
}

size_t memmap::file::size() const {
    if (!is_open()) [[unlikely]]
        throw logic_error("file::size: file is not open");
    assert(fildes_ >= 0);

    if (struct stat buf; fstat(fildes_, &buf) == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "file::size");
    else
        return static_cast<size_t>(buf.st_size);
}

memmap::memmap(
) noexcept : addr_(MAP_FAILED), size_(size_limits::max()), file_() {}

memmap::memmap(const char * const filename) : memmap() {
    open(filename);
    assert(
        (addr_ == MAP_FAILED && size_ == 0) ||
        (addr_ != MAP_FAILED && size_ > 0)
    );
    assert(size_ != size_limits::max() && file_.is_open());
}

memmap::memmap(memmap &&rhs) : memmap() {
    *this = move(rhs);
    assert(
        rhs.addr_ == MAP_FAILED &&
        rhs.size_ == size_limits::max() &&
        !rhs.file_.is_open()
    );
}

memmap &memmap::operator=(memmap &&rhs) {
    if (is_open())
        close();
    assert(
        addr_ == MAP_FAILED &&
        size_ == size_limits::max() &&
        !file_.is_open()
    );

    swap(addr_, rhs.addr_);
    swap(size_, rhs.size_);
    swap(file_, rhs.file_);
    return *this;
}

memmap::~memmap() noexcept {
    if (is_open())
        try {
            close();
        } catch (const std::exception &except) {
#           ifndef NDEBUG
            cerr << except.what() << std::endl;
#           endif
        }
    assert(
        addr_ == MAP_FAILED &&
        size_ == size_limits::max() &&
        !file_.is_open()
    );
}

void memmap::close() {
    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::close: memory map is not open");
    assert(
        (addr_ == MAP_FAILED && size_ == 0) ||
        (addr_ != MAP_FAILED && size_ > 0)
    );
    assert(size_ != size_limits::max() && file_.is_open());

    int errnum = 0;
    if (addr_ != MAP_FAILED && munmap(const_cast<void *>(addr_), size()) == -1)
        errnum = errno;
    addr_ = MAP_FAILED;
    size_ = size_limits::max();

    try {
        file_.close();
    } catch (const std::exception &except) {
        if (errnum == 0)
            errnum = errno;
#       ifndef NDEBUG
        cerr << except.what() << std::endl;
#       endif
    }
    assert(
        addr_ == MAP_FAILED &&
        size_ == size_limits::max() &&
        !file_.is_open()
    );

    if (errnum != 0) [[unlikely]]
        throw system_error(errnum, generic_category(), "memmap::close");
}

const char *memmap::data() const {
    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::data: file is not open");
    return addr_ == MAP_FAILED ? nullptr : static_cast<const char *>(addr_);
}

bool memmap::is_open() const noexcept {
    return file_.is_open();
}

void memmap::open(const char * const filename) {
    if (is_open()) [[unlikely]]
        throw logic_error("memmap::open: memory map is aldready open");
    assert(
        addr_ == MAP_FAILED &&
        size_ == size_limits::max() &&
        !file_.is_open()
    );

    file_.open(filename);
    size_ = 0;

    if (const size_t len = file_.size(); len != 0) {
        assert(len != size_limits::max());
        addr_ = mmap(nullptr, len, PROT_READ, MAP_SHARED, file_.fildes(), 0);
        if (addr_ == MAP_FAILED) [[unlikely]]
            throw system_error(errno, generic_category(), "memmap::open");
        size_ = len;
    }

    assert(
        (addr_ == MAP_FAILED && size_ == 0) ||
        (addr_ != MAP_FAILED && size_ > 0)
    );
    assert(size_ != size_limits::max() && file_.is_open());
}

size_t memmap::size() const {
    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::size: file is not open");
    return size_;
}

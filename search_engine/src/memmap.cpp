#include <cassert>
#include <cerrno>

#include <iostream> // cerr
#include <system_error>
#include <utility> // swap

#include <fcntl.h> // open
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // fstat
#include <unistd.h> // close

#include <search_engine/memmap.hpp>

using std::generic_category, std::logic_error, std::size_t, std::system_error;

inline constexpr file::file() noexcept : fildes_(-1) {}

inline file::file(const char * const filename) : file() {
    open(filename);
    assert(fildes_ >= 0);
}

inline constexpr file::file(file &&rhs) : file() {
    *this = std::move(rhs);
    assert(rhs.fildes_ == -1);
}

inline constexpr file &file::operator=(file &&rhs) {
    if (is_open())
        close();
    assert(fildes_ == -1);

    swap(fildes_, rhs.fildes_);
    return *this;
}

inline file::~file() noexcept {
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

inline void file::close() {
    if (!is_open())
        throw logic_error("file::close: file is not open");
    assert(fildes_ >= 0);

    const int fildes = fildes_;
    fildes_ = -1;
    if (close(fildes) == -1)
        throw system_error(errno, generic_category(), "file::close");
}

inline constexpr int file::fildes() const noexcept {
    if (!is_open())
        throw logic_error("file::fildes: file is not open");
    assert(fildes_ >= 0);

    return fildes_;
}

inline constexpr bool file::is_open() const noexcept {
    return fildes_ >= 0;
}

inline void file::open(const char * const filename) {
    if (is_open())
        throw logic_error("file::open: file is aldready open");
    assert(fildes_ == -1);

    fildes_ = open(filename, O_RDONLY);
    if (fildes_ == -1)
        throw system_error(errno, generic_category(), "file::open");
    assert(fildes_ >= 0);
}

inline off_t file::size() const {
    if (!is_open())
        throw logic_error("file::size: file is not open");
    assert(fildes_ >= 0);

    if (stat buf; fstat(fildes_, &buf) == -1)
        throw system_error(errno, generic_category(), "file::size");
    else
        return buf.st_size;
}

inline memmap::memmap(
) noexcept : off_(-1), size_(-1), addr_(MAP_FAILED), file_() {}

inline memmap::memmap(const char * const filename) : memmap() {
    open(filename);
}

inline const char *memmap::data() const noexcept {
    if (!is_open())
        throw logic_error("memmap::data: file is not open");
    return addr_ == MAP_FAILED ? nullptr : static_cast<const char *>(addr_);
}

inline bool memmap::is_open() const noexcept {
    return file_.is_open();
}

inline size_t memmap::length() const noexcept {
    if (!is_open())
        throw logic_error("memmap::data: file is not open");
    return std::min(max_len, static_cast<size_t>(size_ - off_));
}

inline void memmap::open(const char * const filename) {
    if (is_open())
        throw logic_error("memmap::open: memory map is aldready open");
    assert(off_ == -1 && size_ == -1 && addr_ == MAP_FAILED);

    file_.open(filename);
    size_ = file_.size();
    ;
}

void memmap::seek(const off_t off) {
    if (!is_open())
        throw logic_error("memmap::seek: memory map is not open");
    else if (off < 0 || off > size_)
        throw logic_error("memmap::seek: off is negative or greater than size");
    assert(fildes_ >= 0);
    assert(
        (off_ >= 0 && off_ < size_ && addr_ != MAP_FAILED) ||
        (off_ >= 0 && off_ == size_ && addr_ == MAP_FAILED)
    );

    ;
}

inline off_t memmap::size() const noexcept {
    return size_;
}

inline off_t memmap::tell() const noexcept {
    return off_;
}

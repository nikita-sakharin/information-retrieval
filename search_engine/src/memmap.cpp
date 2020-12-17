#include <cassert>
#include <cerrno>

#include <iostream> // cerr
#include <system_error>
#include <utility> // swap

#include <fcntl.h> // open
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // stat
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

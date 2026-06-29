#include <cerrno> // errno

#include <system_error> // generic_category, system_error

#include <fcntl.h> // O_RDONLY, open
#include <sys/mman.h> // MAP_FAILED, MAP_SHARED, PROT_READ, mmap, munmap
#include <sys/stat.h> // fstat
#include <unistd.h> // close

#include <search_engine/memmap.hpp>

using std::cerr, std::endl, std::exception, std::generic_category,
    std::logic_error, std::size_t, std::system_error;

memmap::memmap() noexcept : addr_(MAP_FAILED) {}

memmap::memmap(memmap &&rhs) : memmap() {
    using std::move;

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

    swap(rhs);
    return *this;
}

memmap::~memmap() noexcept {
    if (is_open())
        try {
            close();
        } catch (const exception &except) {
#           ifndef NDEBUG
            cerr << except.what() << endl;
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
    if (addr_ != MAP_FAILED && munmap(const_cast<void *>(addr_), size_) == -1)
        [[unlikely]] errnum = errno;
    addr_ = MAP_FAILED;
    size_ = size_limits::max();

    try {
        file_.close();
    } catch (const exception &except) {
        if (errnum == 0)
            errnum = errno;
        else {
#           ifndef NDEBUG
            cerr << except.what() << endl;
#           endif
        }
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
        throw logic_error("memmap::data: memory map is not open");
    return addr_ == MAP_FAILED ? nullptr : static_cast<const char *>(addr_);
}

void memmap::open(const char * const filename) {
    if (is_open()) [[unlikely]]
        throw logic_error("memmap::open: memory map is already open");
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

void memmap::file::close() {
    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::file::close: file is not open");
    assert(fildes_ >= 0);

    const int returns = ::close(fildes_);
    fildes_ = -1;
    if (returns == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "memmap::file::close");
}

void memmap::file::open(const char * const filename) {
    if (is_open()) [[unlikely]]
        throw logic_error("memmap::file::open: file is already open");
    assert(fildes_ == -1);

    fildes_ = ::open(filename, O_RDONLY);
    if (fildes_ == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "memmap::file::open");
    assert(fildes_ >= 0);
}

size_t memmap::file::size() const {
    if (!is_open()) [[unlikely]]
        throw logic_error("memmap::file::size: file is not open");
    assert(fildes_ >= 0);

    if (struct stat buf; fstat(fildes_, &buf) == -1) [[unlikely]]
        throw system_error(errno, generic_category(), "memmap::file::size");
    else
        return static_cast<size_t>(buf.st_size);
}

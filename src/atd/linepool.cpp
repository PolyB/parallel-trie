#include "atd/linepool.hpp"

#include <new>
#include <sys/mman.h>
#include <unistd.h> // getpagesize

static const size_t page_size = getpagesize();
static const size_t page_size_mask = page_size - 1;

template <uint32_t POS>
static inline constexpr uint32_t align_with(uint32_t i)
{
  return (i & (POS - 1)) ? (i & ~(POS - 1)) + POS : i;
}

namespace atd
{
  linepool::linepool(uint8_t llen)
    : pages_(),  it_(pages_.begin()), /*page_it_(pages_.begin()), */curr_(nullptr), llen_(align_with<32>(llen))
  {}

  linepool::~linepool()
  {
    for (void* ptr : pages_)
      munmap(ptr, page_size);
  }

  line linepool::alloc()
  {
    static_assert(std::is_trivially_destructible<std::remove_pointer<line>>::value);
    static_assert(std::is_trivially_constructible<std::remove_pointer<line>>::value);

    if (!(reinterpret_cast<size_t>(curr_) & page_size_mask)) // no space left in the page
    {
      if (it_ != pages_.end())
      {
        curr_ = reinterpret_cast<char*>(*(it_++));
      }
      else // it_ == pages_curr_.end()
      {
        void *page = mmap(nullptr, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (page == MAP_FAILED)
          throw std::bad_alloc();
        pages_.push_front(page);
        curr_ = reinterpret_cast<char*>(page);
      }
    }

    char* oldcurr = curr_;
    curr_ += llen_;
    return reinterpret_cast<line>(oldcurr);
  }

  void linepool::reset()
  {
    it_ = pages_.begin();
  }

} // namespace atd

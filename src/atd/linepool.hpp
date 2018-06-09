#pragma once

#include <cstdint>
#include <forward_list>
#include <iterator>

#include "atd/line.hpp"

namespace atd
{

class linepool
{
  public:
    explicit linepool(uint8_t llen);
    line alloc();
    void reset();
    ~linepool();
  private:
    std::forward_list<void*> pages_;
    decltype(pages_)::iterator it_;

    char *curr_;
    uint8_t llen_;
};

} // namespace atd

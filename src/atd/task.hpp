#pragma once

#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

#include "atd/advanced_trie.hpp"
#include "atd/linepool.hpp"

namespace atd
{

class task_stack;

class task
{
  public:
    task(std::string_view, const advanced_trie *, atd::linepool*);
    task(std::string_view target, atd::line line,
                  atd::line prev, char last_char,
                  const advanced_trie* node, uint8_t min, atd::linepool*);

    std::pair<uint8_t, const advanced_trie*> operator()(task_stack*);
  private:

    std::string_view target_;
    atd::linepool* pool_;
    atd::line line_;
    atd::line prev_;
    const advanced_trie* node_;
    char last_char_;
    uint8_t min_;

};

} // namespace atd

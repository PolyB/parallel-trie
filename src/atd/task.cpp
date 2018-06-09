#include "atd/task.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

#include "atd/task_stack.hpp"

namespace atd
{

task::task(std::string_view target, atd::line line,
                atd::line prev, char last_char,
                const advanced_trie* node, uint8_t min, atd::linepool* pool)
  : target_(target), pool_(pool), line_(line), prev_(prev), node_(node), last_char_(last_char), min_(min)
{}

task::task(std::string_view target, const advanced_trie* node, atd::linepool* pool)
  : target_(target), pool_(pool), node_(node), last_char_('\0'), min_(0)
{
  line_ = pool->alloc();
  prev_ = pool->alloc();

  std::iota(line_, line_ + target.size() + 1, 0);
}

std::pair<uint8_t, const advanced_trie*> task::operator()(task_stack* ts)
{
  node_->foreach(
      [this, ts] (const advanced_trie& child) {
        atd::line c_line = pool_->alloc();

        c_line[0] = line_[0] + 1;
        uint8_t j = c_line[0]; // current iteration
        uint8_t c_min = j;
        char c = child.get_char();
        for (uint8_t i = 1; i <= target_.size(); i++)
        {
          if (c == target_[i - 1])
            c_line[i] = line_[i-1];
          else
          {
            uint8_t tmp = std::min({c_line[i - 1], line_[i], line_[i - 1]});
            if (j >= 2 && i >= 2 && target_[i-1] == last_char_ && target_[i-2] == c)
              tmp = std::min({prev_[i - 2], tmp});
            c_line[i] = 1 + tmp;
          }
          c_min = std::min(c_min, c_line[i]);
        }
        ts->push(c_min != min_, std::make_unique<task>(target_, c_line, line_, c, &child, c_min, pool_));
      });
  if (node_->is_word_end())
    return std::make_pair(line_[target_.size()], node_);
  else
    return std::make_pair(-1, nullptr);
}

} // namespace atd

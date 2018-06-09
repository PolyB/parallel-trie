#pragma once
#include <array>
#include <memory>

namespace btd
{

class basic_trie
{
  public:
    basic_trie();
    void add(const char*);
    void remove(const char*);
    bool is_word_end() const { return word_end_; }

    using childs_t = std::array<std::pair<char, std::unique_ptr<basic_trie>>, 'z' - 'a' + 1>;

     childs_t childs;
  private:
    childs_t::iterator find_child(char);
    bool word_end_;
};

} // namespace btd

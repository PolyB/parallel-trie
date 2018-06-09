#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <memory>

namespace atd
{

class advanced_trie
{
  public:
    explicit advanced_trie(advanced_trie* parent = nullptr, char c = '\0');
    void add(const char*);
    void remove(const char*);
    bool is_word_end() const { return word_end_.load(std::memory_order_relaxed); } // mem_ord : we don't need to pull changes
    char get_char() const { return my_char_; }

    template <class F>
    void foreach(F&&) const;

    std::string get_word() const;

    using child_t = advanced_trie;
    using childs_t = std::array<std::atomic<child_t*>, 'z' - 'a' + 1>;

  private:
		static child_t*
      add_create_nodes(const char* str, advanced_trie* parent);

    static void advance_tries(childs_t::iterator&, advanced_trie*& new_branch, advanced_trie*& allocated_branch);

    childs_t::iterator find_child_relaxed(char);
    childs_t::iterator find_child_acquire(char);

    childs_t childs_;
    advanced_trie* parent_;
    char my_char_; // only if there is a parent
    std::atomic<bool> word_end_;
};

template <class F>
void advanced_trie::foreach(F&& f) const
{
  // mem_ord : not in insert / removal, we don't care
  for (const auto& child : childs_)
  {
    if (!child.load(std::memory_order_acquire))
      if (!child.load(std::memory_order_relaxed))
        break;
    f(*child.load(std::memory_order_relaxed));
  }
}

} // namespace atd

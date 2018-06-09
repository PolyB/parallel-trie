#include "advanced_trie.hpp"

#include <algorithm>
#include <iostream>

namespace atd
{

advanced_trie::advanced_trie(advanced_trie* p, char c)
  : childs_()
	, parent_(p)
  , my_char_(c)
  , word_end_(false)
{}

advanced_trie::childs_t::iterator advanced_trie::find_child_relaxed(char c)
{
  auto childit = childs_.begin();
    for (;childit->load(std::memory_order_relaxed); childit++)
      if (childit->load(std::memory_order_relaxed)->my_char_ == c)
        return childit;
  return childit;
}

advanced_trie::childs_t::iterator advanced_trie::find_child_acquire(char c)
{
  auto childit = childs_.begin();
    for (;childit->load(std::memory_order_acquire); childit++)
      if (childit->load(std::memory_order_relaxed)->my_char_ == c)
        return childit;
  return childit;
}

advanced_trie::child_t*
  advanced_trie::add_create_nodes(const char* str, advanced_trie* parent)
{
  auto res = new child_t(parent, *str);

  if (!*(str + 1)) // empty word
    res->word_end_.store(true, std::memory_order_relaxed); // legal ?
  else // we just created this node so it does not have child
	  res->childs_[0].store(add_create_nodes(str + 1, res), std::memory_order_release);
	return res;
}

void advanced_trie::advance_tries(childs_t::iterator& childit, advanced_trie*& new_branch, advanced_trie*& allocated_branch)
{
  do
  {
    if (allocated_branch->word_end_.load(std::memory_order_relaxed)) // allocated branch was allocated by this thread -> relaxed
    {
      childit->load(std::memory_order_relaxed)->word_end_.store(true, std::memory_order_relaxed); // release ?
      delete allocated_branch;
      allocated_branch = nullptr;
      return;
    }
    auto old_allocated_branch = allocated_branch;
    allocated_branch = allocated_branch->childs_[0].load(std::memory_order_relaxed); // allocated branch is thread_local for now
    delete old_allocated_branch;
    childit = new_branch->find_child_acquire(allocated_branch->childs_[0].load(std::memory_order_relaxed)->my_char_);
    new_branch = childit->load(std::memory_order_relaxed); // already got memory order from find_child
  }
  while (new_branch);
}

void advanced_trie::add(const char* str)
{
  if (!*str) // empty word
    word_end_.store(true, std::memory_order_release);
  else
  {
    auto childit = find_child_acquire(*str);
    if (!childit->load(std::memory_order_relaxed)) // not found : (memory order acquired from find_child_acquire
		{
      auto temp = add_create_nodes(str, this);
      advanced_trie* new_trie_expected = nullptr;
		  while (!childit->compare_exchange_strong(new_trie_expected, temp, // TODO : test weak
            std::memory_order_acq_rel, std::memory_order_acquire)) // TODO : thread senitizer test
      {
        if (!new_trie_expected)
          continue;
        // on s'est fait prank
        if (new_trie_expected->my_char_ == *str) // insert the same character
        {
          advanced_trie::advance_tries(childit, new_trie_expected, temp);
          if (!temp)
            return;
        }
        else
          childit++;
      }

		}
		else
      childit->load(std::memory_order_relaxed)->add(str + 1);
  }
}

// note: we are not deallocating the useless branches (for now)
void advanced_trie::remove(const char *str)
{
  if (!*str)
    word_end_.store(false, std::memory_order_relaxed);
  else
  {
    auto childit = find_child_relaxed(*str);
    if (*childit) // found
      (**childit).remove(str + 1);
  }
}

std::string advanced_trie::get_word() const
{
  std::string s;
  for (const advanced_trie *at = this; at->parent_; at = at->parent_)
    s.push_back(at->my_char_);
  std::reverse(s.begin(), s.end());
  return s;
}

} // namespace atd

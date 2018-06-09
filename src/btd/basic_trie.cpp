#include "btd/basic_trie.hpp"

namespace btd
{

basic_trie::basic_trie()
  :childs({std::make_pair('\0', nullptr)})
  , word_end_(false)
{}

basic_trie::childs_t::iterator basic_trie::find_child(char c)
{
  auto childit = childs.begin();
  for (;childit->first;childit++)
    if (childit->first == c)
      return childit;
  return childit;
}

void basic_trie::add(const char *str)
{
  if (!*str) // empty word
    word_end_ = true;
  else
  {
    auto childit = find_child(*str);
    if (!childit->first) // not found
      *childit = std::make_pair(*str, std::make_unique<basic_trie>());
    childit->second->add(str + 1);
  }
}

// note: we are not deallocating the useless branches (for now)
void basic_trie::remove(const char *str)
{
  if (!*str)
    word_end_ = false;
  else
  {
    auto childit = find_child(*str);
    if (childit->first) // found
      childit->second->remove(str + 1);
  }
}

} // namespace btd

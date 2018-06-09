#include "advanced_trie_dictionary.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>

#include "atd/task_stack.hpp"
#include "atd/task.hpp"
#include "atd/linepool.hpp"

advanced_trie_dictionary::advanced_trie_dictionary()
  : trie_(std::make_unique<atd::advanced_trie>())
{}

advanced_trie_dictionary::advanced_trie_dictionary(const std::initializer_list<std::string>& init)
  :advanced_trie_dictionary(init.begin(), init.end())
{}


void advanced_trie_dictionary::init(const std::vector<std::string>& word_list)
{
  trie_ = std::make_unique<atd::advanced_trie>();
  for (const auto& word : word_list)
    trie_->add(word.c_str());
}

void advanced_trie_dictionary::insert(const std::string& w)
{
  trie_->add(w.c_str());
}

void advanced_trie_dictionary::erase(const std::string& w)
{
  trie_->remove(w.c_str());
}

std::pair<uint8_t, const atd::advanced_trie*> advanced_trie_dictionary::do_search(const std::string& s) const
{
  static thread_local atd::linepool pool(s.size() + 1);
  atd::task_stack ts;
	// Push first task in stack
  auto min = std::make_pair<uint8_t, atd::advanced_trie*>(-1, nullptr);
  uint8_t it = 0;

  ts.push(0, std::make_unique<atd::task>(s, trie_.get(), &pool));
	
	do
	{
		while (!ts.empty(0))
		{
			auto atask = ts.pop();
      auto result = (*atask)(&ts);
      if (result.first == it) // perfect match
      {
        pool.reset();
        return result;
      }

      if (result.first < min.first) // normal match
        result = min;
		}
		ts.swap_tasks();
    it++;
	} while (min.first != it && !ts.empty(0));
  pool.reset();
  return min;
}

result_t advanced_trie_dictionary::search(const std::string& w) const
{
  //std::lock_guard l(m);
  auto r = do_search(w);
	if (!r.second)
	  return {"", std::numeric_limits<int>::max()};
  return {r.second->get_word(), r.first};
}

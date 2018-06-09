#include "basic_trie_dictionary.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>


basic_trie_dictionary::basic_trie_dictionary()
  : trie_(std::make_unique<btd::basic_trie>())
{}

basic_trie_dictionary::basic_trie_dictionary(const std::initializer_list<std::string>& init)
  :basic_trie_dictionary(init.begin(), init.end())
{}

void basic_trie_dictionary::init(const std::vector<std::string>& word_list)
{
  trie_ = std::make_unique<btd::basic_trie>();
  for (const auto& word : word_list)
  {
    trie_->add(word.c_str());
  }
}

void basic_trie_dictionary::insert(const std::string& w)
{
  trie_->add(w.c_str());
}

void basic_trie_dictionary::erase(const std::string& w)
{
  trie_->remove(w.c_str());
}

std::pair<std::string, uint8_t> basic_trie_dictionary::do_search(const std::string& target,
  const std::vector<uint8_t>& prev, const std::vector<uint8_t>& pprev,
	std::pair<char, char> curr, const btd::basic_trie* trie)
{
  std::vector<uint8_t> line(target.size() + 1, 0);
  line[0] = prev[0] + 1;
  uint8_t j = line[0];
  for (uint8_t i = 1; i <= target.size(); i++)
  {
    if (curr.first == target[i-1])
      line[i] = prev[i-1];
    else
    {
      uint8_t tmp = std::min({line[i-1], prev[i], prev[i-1]});
      // Test swap
      if (j >= 2 && i >= 2 && target[i-1] == curr.second && target[i-2] == curr.first)
        tmp = std::min({pprev[j - 2], tmp});
      line[i] = 1 + tmp;
    }
  }


  auto best = std::make_pair<std::string, uint8_t>("", -1);
  if (trie->is_word_end())
    best = {{curr.first}, line[target.size()]};
  // pars, cours !
  // des fils
  for (const auto& child : trie->childs)
  {
    if (!child.first)
      break;
    auto obest = do_search(target, line, prev, std::make_pair(child.first, curr.second), child.second.get());
    obest.first += curr.first;
    if (obest.second < best.second)
      best = obest;
  }
  return best;
}

result_t basic_trie_dictionary::search(const std::string& w) const
{
  std::vector<uint8_t> pline(w.size() + 1, 0);
  std::vector<uint8_t> line(w.size() + 1, 0);

  std::iota(line.begin(), line.end(), 0);

  std::pair<std::string, uint8_t> flem2 = {"", -1};

  for (const auto& child : trie_->childs)
  {
    if (!child.first)
      break;
    auto rene = do_search(w, line, pline, {child.first, '\0'}, child.second.get());
    if (rene.second < flem2.second)
      flem2 = std::move(rene);
  }

  // auto flem = do_search(w, line, pline, "", trie_.get());
  std::reverse(flem2.first.begin(), flem2.first.end());
  return flem2;
}

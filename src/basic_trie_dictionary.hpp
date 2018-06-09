#pragma once

#include <memory>

#include "IDictionary.hpp"
#include "btd/basic_trie.hpp"

class basic_trie_dictionary : public IDictionary
{
public:
  basic_trie_dictionary();
  basic_trie_dictionary(const std::initializer_list<std::string>& init);

  template <class Iterator>
  basic_trie_dictionary(Iterator begin, Iterator end);

  void init(const std::vector<std::string>& word_list) final;


  result_t      search(const std::string& w) const final;
  void          insert(const std::string& w) final;
  void          erase(const std::string& w) final;

private:
  static std::pair<std::string, uint8_t> do_search(const std::string& target,
	  const std::vector<uint8_t>& prev, const std::vector<uint8_t>& pprev,
	  std::pair<char, char> curr, const btd::basic_trie* trie);
  std::unique_ptr<btd::basic_trie> trie_;
};

template <class Iterator>
basic_trie_dictionary::basic_trie_dictionary(Iterator begin, Iterator end)
  :trie_(std::make_unique<btd::basic_trie>())
{
  for (auto it = begin; it != end;it++)
  {
    trie_->add(it->data());
  }
}

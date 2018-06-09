#pragma once

#include <memory>
#include <mutex>

#include "IDictionary.hpp"
#include "atd/advanced_trie.hpp"

class advanced_trie_dictionary : public IDictionary
{
public:
  advanced_trie_dictionary();
  advanced_trie_dictionary(const std::initializer_list<std::string>& init);

  template <class Iterator>
  advanced_trie_dictionary(Iterator begin, Iterator end);

  void init(const std::vector<std::string>& word_list) final;


  result_t      search(const std::string& w) const final;
  void          insert(const std::string& w) final;
  void          erase(const std::string& w) final;

private:
  std::pair<uint8_t, const atd::advanced_trie*> do_search(const std::string&) const;

  mutable std::mutex m;
  std::unique_ptr<atd::advanced_trie> trie_;
};

template <class Iterator>
advanced_trie_dictionary::advanced_trie_dictionary(Iterator begin, Iterator end)
  :trie_(std::make_unique<atd::advanced_trie>())
{
  for (auto it = begin; it != end;it++)
    trie_->add(it->data());
}

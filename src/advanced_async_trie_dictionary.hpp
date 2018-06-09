#pragma once

#include "IAsyncDictionary.hpp"
#include "advanced_trie_dictionary.hpp"


/// The naive implementation is blocking to ensure Sequential Consistency
class advanced_async_trie_dictionary : public IAsyncDictionary
{
public:
  advanced_async_trie_dictionary() = default;

  template <class Iterator>
  advanced_async_trie_dictionary(Iterator begin, Iterator end);
  advanced_async_trie_dictionary(const std::initializer_list<std::string>& init);
  void init(const std::vector<std::string>& word_list) final;

  std::future<result_t>      search(const std::string& w) const final;
  std::future<void>          insert(const std::string& w) final;
  std::future<void>          erase(const std::string& w) final;

private:
  advanced_trie_dictionary m_dic;
};

template <class Iterator>
advanced_async_trie_dictionary::advanced_async_trie_dictionary(Iterator begin, Iterator end)
  : m_dic(begin, end)
{
}

#pragma once
#include "IDictionary.hpp"
#include "IAsyncDictionary.hpp"
#include <string>
#include <vector>
#include <memory>

///! Compute the Optimal string alignment distance between two strings
int levenshtein(const std::string& a, const std::string& b);


///! Load the word list from a file
std::vector<std::string> load_word_list(const char* filename = nullptr, bool shuffle = true);



class Scenario
{
public:
  ~Scenario();
  Scenario();
  Scenario(Scenario&&) = default;

  // Create a scenario from a word list
  Scenario(const std::vector<std::string>& word_list, std::size_t nqueries);

  // Prepare and populate the dictionary
  void prepare(IDictionary& dic) const;
  void prepare(IAsyncDictionary& dic) const;

  // Execute a scenario sync-way
  std::vector<result_t> execute(IDictionary& dic) const;

  // Execute a scenario async-way
  std::vector<result_t> execute(IAsyncDictionary& dic) const;

  void execute_verbose(IDictionary& dic) const;

private:
  struct scenario_impl_t;
  std::unique_ptr<scenario_impl_t> m_impl;
};



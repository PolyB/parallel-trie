# Parallel trie

This repository contains an implementation of a native trie implementation `naive_dictionary` and a lock-free optimized trie implementation `advanced_trie_dictionary`

## optimisations
The main optimisations are:
### Data structures
The data structure is built to be lock-free, memory-alligned and have light memory usage complexity
### Memory otimisations
The search in the trie is done using a memory pool,
the memory pool gives us sevral advantages :
* we are not using the standard C++ library for allocations
* we are not using `std::shared_ptr` because each thread has it's own memory pool (we don't need thread safety) and we flush the pool when the search is done (we don't need reference counting)
* we do not have to call destructors

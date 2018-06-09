#include "atd/task_stack.hpp"

#include <iostream>

#include "atd/task.hpp"

namespace atd
{

std::unique_ptr<task> task_stack::pop()
{
	if (stacks_[0].size() == 0)
	  return nullptr;
	
	std::unique_ptr t = std::move(stacks_[0].back());
	stacks_[0].pop_back();
	return t;
}

void task_stack::push(int stack, std::unique_ptr<task> t)
{
  stacks_[stack].emplace_back(std::move(t));
}

void task_stack::swap_tasks()
{
  std::swap(stacks_[0], stacks_[1]);
}

} // namespace atd

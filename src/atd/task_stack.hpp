#pragma once

#include <memory>
#include <utility>
#include <vector>

namespace atd
{

class task;

class task_stack
{
	public:
	  task_stack() = default;
		
		std::unique_ptr<task> pop();
		void push(int stack, std::unique_ptr<task>);
		void swap_tasks();
		bool empty(int stack)
		{ return stacks_[stack].size() == 0; }

	private:
		std::array<std::vector<std::unique_ptr<task>>, 2> stacks_;
};


} // namespace atd

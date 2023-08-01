#include <vector>
#include <iostream>
#include <stack>
#include <utility>

void subsets_helper(const std::vector<int>& input, 
		std::vector<int>& curr, 
		int idx, 
		std::vector<std::vector<int>>& output) 
{
	if (idx == input.size()) {
		output.push_back(curr);
		return;
	}

	curr.push_back(input[idx]);
	subsets_helper(input, curr, idx + 1, output);
	curr.pop_back();
	subsets_helper(input, curr, idx + 1, output);
	
}

std::vector<std::vector<int>> subsets_recursive(const std::vector<int>& input) 
{
	std::vector<std::vector<int>> output;
	std::vector<int> current;

	int idx = 0;
	subsets_helper(input, current, idx, output);
	return output;
}

std::vector<std::vector<int>> subsets_iterative_1(
	const std::vector<int>& input
) {
	std::vector<std::vector<int>> output;
	std::stack<std::pair<int, std::vector<int>>> stack;
	stack.push({ 0, {} });

	while (!stack.empty()) {
		std::pair<int, std::vector<int>> top = stack.top();
		stack.pop();

		int idx = top.first;
		std::vector<int> current = top.second;

		if (idx == input.size()) {
			output.push_back(current);
			continue;
		}

		stack.push({ idx + 1, current });

		current.push_back(input[idx]);

		stack.push({ idx + 1, current });
	}

	return output;
}

std::vector<std::vector<int>> subsets_iterative_2(
	const std::vector<int>& input
) {
	std::vector<std::vector<int>> output;
	output.push_back({});

	for (int i = 0; i < input.size(); ++i) {
		int N = output.size();
		for (int j = 0; j < N; ++j) {
			std::vector<int> current = output[j];
			current.push_back(input[i]);
			output.push_back(current);
		}
	}

	return output;
}


int main() {
	
	std::vector<int> input = { 1, 2, 3, 4 };
	//std::vector<std::vector<int>> output = subsets_recursive(input);

	//std::vector<std::vector<int>> output = subsets_iterative_1(input);

	std::vector<std::vector<int>> output = subsets_iterative_2(input);

	for (int i = 0; i < output.size(); ++i) {
		std::cout << "{";
		const std::vector<int>& subset = output[i];
		for (int j = 0; j < subset.size(); ++j) {
			std::cout << subset[j] << ",";
		}
		std::cout << "}" << std::endl;
	}

	return 0;
}
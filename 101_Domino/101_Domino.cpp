#include <iostream>
#include <array>
#include <list>
#include <utility>
#include <tuple>
#include <vector>

using DominoType = std::tuple<int, int, int, bool, std::vector<int>,
                              std::vector<int>>;

bool checkIfNoSolution(const std::vector<DominoType>& dominos, int leftSide, int rightSide)
{
	std::array<int, 7> values{};

	values[leftSide]++;
	values[rightSide]++;

	for (auto it = dominos.begin(); it != dominos.end(); ++it)
	{
		if (std::get<3>(*it) == true)
			continue;

		values[std::get<0>(*it)]++;
		values[std::get<1>(*it)]++;
	}

	int oddValuesCount = 0;

	for (const int& value : values)
	{
		if (value % 2 == 1)
			++oddValuesCount;

		if (oddValuesCount > 2)
			return true;
	}

	return oddValuesCount != 0 && oddValuesCount != 2;
}

bool arrange(const int left_side, const int right_side, std::vector<DominoType>& dominos,
             std::list<std::pair<DominoType, bool>>& dominosArranged)
{
	if (dominosArranged.size() == dominos.size())
		return true;

	if (checkIfNoSolution(dominos, left_side, right_side))
		return false;

	for (auto it = dominos.begin(); it != dominos.end(); ++it)
	{
		auto& domino = *it;

		if (std::get<3>(domino) == true)
			continue;

		const auto dominoLeft = std::get<0>(domino);
		const auto dominoRight = std::get<1>(domino);

		if (dominoRight == left_side)
		{
			dominosArranged.emplace_front(domino, false);
			std::get<3>(domino) = true;

			if (arrange(dominoLeft, right_side, dominos, dominosArranged))
				return true;

			dominosArranged.pop_front();
			std::get<3>(domino) = false;
		}
		else if (dominoLeft == left_side)
		{
			dominosArranged.emplace_front(domino, true);
			std::get<3>(domino) = true;

			if (arrange(dominoRight, right_side, dominos, dominosArranged))
				return true;

			dominosArranged.pop_front();
			std::get<3>(domino) = false;
		}
		else if (dominoLeft == right_side)
		{
			dominosArranged.emplace_back(domino, false);
			std::get<3>(domino) = true;

			if (arrange(left_side, dominoRight, dominos, dominosArranged))
				return true;

			dominosArranged.pop_back();
			std::get<3>(domino) = false;
		}
		else if (dominoRight == right_side)
		{
			dominosArranged.emplace_back(domino, true);
			std::get<3>(domino) = true;

			if (arrange(left_side, dominoLeft, dominos, dominosArranged))
				return true;

			dominosArranged.pop_back();
			std::get<3>(domino) = false;
		}
	}

	return false;
}

int main()
{
	int N;
	std::cin >> N;

	std::vector<std::tuple<int, int>> dominosOriginal;

	std::vector<DominoType> dominos;

	std::array<std::vector<int>, 7> doubles{};

	int doubles_count = 0;

	short last_double_index = -1;

	for (int i = 0; i < N; ++i)
	{
		short left, right;

		std::cin >> left;
		std::cin >> right;

		dominosOriginal.emplace_back(left, right);

		if (left == right)
		{
			doubles[left].push_back(i);
			++doubles_count;

			if (last_double_index != -2 && last_double_index != left)
			{
				if (last_double_index == -1)
					last_double_index = left;
				else
					last_double_index = -2;			
			}
			continue;
		}

		dominos.emplace_back(left, right, i, false, std::vector<int>(), std::vector<int>());
	}

	if (dominos.empty())
	{		
		if (last_double_index == -2)
		{
			std::cout << "No solution";

		}
		else
			for (int i = 0; i < N; ++i)
				std::cout << i + 1 << " +" << '\n';

		return 0;
	}
	

	for (int i = 0; i < 7; ++i)
	{
		if (doubles[i].empty())
			continue;

		bool found = false;

		for (auto& domino : dominos)
		{
			if (std::get<0>(domino) == i)
			{
				std::get<4>(domino) = doubles[i];
				found = true;
				break;
			}
			if (std::get<1>(domino) == i)
			{
				std::get<5>(domino) = doubles[i];
				found = true;
				break;
			}
		}

		if (!found)
		{
			std::cout << "No solution";
			return 0;
		}
	}

	std::list<std::pair<DominoType, bool>> dominosArranged;

	dominosArranged.emplace_back(dominos[0], false);

	int leftSide = std::get<0>(dominos[0]);
	int rightSide = std::get<1>(dominos[0]);

	std::get<3>(dominos[0]) = true;
	
	if (checkIfNoSolution(dominos, leftSide, rightSide))
	{
		std::cout << "No solution";
		return 0;
	}

	if (arrange(leftSide, rightSide, dominos, dominosArranged))
	{
		std::vector<std::pair<int, bool>> result_chain;

		for (const auto dominoArranged : dominosArranged)
		{
			const std::tuple<int, int, int, bool, std::vector<int>, std::vector<int>> domino = dominoArranged.first;
			const bool rotated = dominoArranged.second;

			const int dominoIndex = std::get<2>(domino);

			std::vector<int> leftSideDoubles = std::get<4>(domino);
			std::vector<int> rightSideDoubles = std::get<5>(domino);

			if (rotated)
				std::swap(leftSideDoubles, rightSideDoubles);

			if (!leftSideDoubles.empty())
				for (int index : leftSideDoubles)
					result_chain.emplace_back(index, false);			

			result_chain.emplace_back(dominoIndex, rotated);

			if (!rightSideDoubles.empty())
				for (int index : rightSideDoubles)
					result_chain.emplace_back(index, rotated);
		}

		//Chain validation
		/*
		int rightSide = -1;
		for (const auto dominoIndex : result_chain)
		{
			const auto domino = dominosOriginal[std::get<0>(dominoIndex)];
			const bool rotated = std::get<1>(dominoIndex);

			if (rightSide == -1)
			{
				rightSide = rotated ? std::get<0>(domino) : std::get<1>(domino);
			}
			else
			{
				auto leftSide = rotated ? std::get<1>(domino) : std::get<0>(domino);
				if (rightSide != leftSide)
					throw "the chain is wrong";

				rightSide = rotated ? std::get<0>(domino) : std::get<1>(domino);
			}
		}*/

		for (const auto dominoIndex : result_chain)
		{
			std::cout << std::get<0>(dominoIndex) + 1 << ' ' << (std::get<1>(dominoIndex) ? '-' : '+') << '\n';
		}
	}
	else
	{
		std::cout << "No solution";		
	}

	return 0;
}

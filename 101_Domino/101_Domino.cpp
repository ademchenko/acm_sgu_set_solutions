#include <iostream>
#include <array>
#include <list>
#include <utility>
#include <tuple>
#include <vector>

using DominoType = std::tuple<unsigned short, unsigned short, unsigned short, bool>;

bool checkIfNoSolution(const std::vector<DominoType>& dominos, unsigned short leftSide, unsigned short rightSide)
{
	std::array<unsigned short, 7> values {};

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

	for (const unsigned short& value : values)
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
	unsigned short N;
	std::cin >> N;			

	std::vector<DominoType> dominos(N);	

	for (int i = 0; i < N; ++i)
	{
		unsigned short left, right;

		std::cin >> left;
		std::cin >> right;

		dominos[i] = std::make_tuple(left, right, i, false);
	}	

	std::list<std::pair<DominoType, bool>> dominosArranged;

	dominosArranged.emplace_back(dominos[0], false);

	unsigned short leftSide = std::get<0>(dominos[0]);
	unsigned short rightSide = std::get<1>(dominos[0]);

	std::get<3>(dominos[0]) = true;

	if (checkIfNoSolution(dominos, leftSide, rightSide))
	{
		std::cout << "No solution";
		return 0;
	}

	if (arrange(leftSide, rightSide, dominos, dominosArranged))
	{
		for (const auto dominoArranged : dominosArranged)
		{
			const auto domino = dominoArranged.first;
			const auto rotated = dominoArranged.second;

			const int dominoIndex = std::get<2>(domino) + 1;

			std::cout << dominoIndex << ' ' << (rotated ? '-' : '+') << '\n';
		}
	}
	else
	{		
		std::cout << "No solution";
	}

	return 0;
}

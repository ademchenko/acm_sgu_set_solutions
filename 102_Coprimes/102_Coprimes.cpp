#include <iostream>
#include <numeric>

int main()
{
	int N;
	std::cin >> N;

	if (N == 1)
	{
		std::cout << 1;
		return 0;
	}

	int counter = 0;

	for (int i = 1; i < N; ++i)
	{
		if (std::gcd(N, i) == 1)
			++counter;			
	}

	std::cout << counter;
}

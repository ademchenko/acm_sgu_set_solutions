#include <iostream>

//gcd is defined as in Knuth "The Art of Computer Programming" volume 2.
//The algorithm itself is the one defined by Knuth as "Modern Euclidean algorithm":
int gcd(int u, int v)
{
	if (v > u)
		std::swap(u, v);
	
	if (v == 0)
		return u;

	if (v < 0)
	{
		v = -v;
		if (u < 0)
			u = -u;
	}

	int r;
	while ((r = u % v) > 0)
	{
		u = v;
		v = r;
	}

	return v;
}

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
		if (gcd(N, i) == 1)
			++counter;			
	}
	
	std::cout << counter;
}

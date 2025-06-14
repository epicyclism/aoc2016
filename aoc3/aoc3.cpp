#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

using tri_t = std::array<int, 3>;

void sort_tri(int& a, int& b, int& c)
{
	if(a > b)
		std::swap(a, b);
	if(b > c)
		std::swap(b, c);
	if(a > b)
		std::swap(a, b);
}

bool tri_good(int a, int b, int c)
{
	sort_tri(a, b, c);
	return c < a + b;
}

auto get_input()
{
	std::vector<int> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		if(auto[m, a, b, c] = ctre::match<"[ ]+(\\d+)[ ]+(\\d+)[ ]+(\\d+)">(ln); m)
		{
			r.emplace_back(a.to_number<int>());
			r.emplace_back(b.to_number<int>());
			r.emplace_back(c.to_number<int>());
		}
	return r;
}

int pt1(auto const& in)
{
	timer t("p1");
	int r = 0;
	for(auto i = in.begin(); i != in.end(); i += 3)
		r += tri_good(*i, *(i + 1), *(i + 2));
	return r;
}

int pt2(auto const& in)
{
	timer t("p2");
	int r = 0;
	for(auto i = in.begin(); i != in.end(); i += 9)
	{
		r += tri_good(*i, *(i + 3), *(i + 6));
		r += tri_good(*(i + 1), *(i + 4), *(i + 7));
		r += tri_good(*(i + 2), *(i + 5), *(i + 8));
	}
	return r;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

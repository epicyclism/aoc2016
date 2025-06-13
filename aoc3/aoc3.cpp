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

void sort_tri(tri_t& tt)
{
	if(tt[0] > tt[1])
		std::swap(tt[0], tt[1]);
	if(tt[1] > tt[2])
		std::swap(tt[1], tt[2]);
	if(tt[0] > tt[1])
		std::swap(tt[0], tt[1]);
}

bool tri_good(tri_t tt)
{
	sort_tri(tt);
	return tt[2] < tt[0] + tt[1];
}

auto get_input()
{
	std::vector<tri_t> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		if(auto[m, a, b, c] = ctre::match<"[ ]+(\\d+)[ ]+(\\d+)[ ]+(\\d+)">(ln); m)
		{
			tri_t tt{a.to_number<int>(), b.to_number<int>(), c.to_number<int>()};
			r.emplace_back(tt);
		}
	return r;
}

int pt1(auto const& in)
{
	timer t("p1");
	return std::ranges::count_if(in, tri_good);
}

int pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

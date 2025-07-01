#include <iostream>
#include <string>
#include <algorithm>

#include <fmt/format.h>
#include <bit>

#include "ctre_inc.h"
#include "graph.h"
#include "timer.h"

bool is_wall(unsigned x, unsigned y, unsigned special)
{
	return std::popcount(
		x * x +
		3 * x + 
		2 * x * y +
		y +
		y * y +
		special
	) & 1;
}

std::pair<int, int> pt12(auto in)
{
	timer t("p1");
	const int stride = 64;
	grid_virtual g(stride, [=](auto f, auto t){ return !is_wall(t % stride, t / stride, in);});
	auto rv = bfs(g, stride + 1);
	return {rv[31 + 39 * stride],
		std::ranges::count_if(rv, [](auto d){ return d != -1 && d < 51;})};
}

int main(int ac, char ** av)
{
	int in = 1362;
	if(ac > 1)
		in = sv_to_t<int>(std::string_view(av[1]));
	auto[ p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

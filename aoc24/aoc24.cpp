#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

struct grid
{
	std::vector<char> grid_;
	size_t stride_;
	std::vector<size_t>  pts_;
};

auto get_input()
{
	grid g;
	std::string ln;
	std::getline(std::cin, ln);
	g.stride_ = ln.size();
	auto pt = ln.size();
	g.grid_.append_range(ln);
	while(std::getline(std::cin, ln))
	{
		for(int n = 0; n < ln.size(); ++n)
			if(::isdigit(ln[n]))
			{
				if(ln[n] == '0')
					g.pts_.insert(g.pts_.begin(), n + pt);
				g.pts_.emplace_back(n + pt);
			}
		g.grid_.append_range(ln);
	}
	return g;
}

int64_t pt1(auto const& in_addr_t)
{
	timer t("p1");
	return 0;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

void dmp(auto const& in)
{
	fmt::println("grid size = {}, stride = {}, found {} digits, 0 at {}", in.grid_.size(), in.stride_, in.pts_.size(), in.pts_[0]);
}

int main()
{
	auto in = get_input();
	dmp(in);
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

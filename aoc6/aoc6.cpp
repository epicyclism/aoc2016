#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

using ft_t = std::array<int, 26>;
using ftt_t = std::array<ft_t, 8>;

auto get_input()
{
	std::vector<std::string> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		r.emplace_back(std::move(ln));
	return r;
}

auto build_ftt(auto const& in)
{
	ftt_t ftt;
	for(auto& ff: ftt)
		ff.fill(0);
	for(auto& s: in)
	{
		int n = 0;
		for(auto c: s)
		{
			++ftt[n][c - 'a'];
			++n;
		}
	}
	return ftt;
}

auto pt12(auto const& in)
{
	timer t("p1");
	std::string m1 = "aaaaaaaa";
	std::string m2 = "aaaaaaaa";
	auto ftt = build_ftt(in);
	for(int n = 0; n < 8; ++n)
	{
		m1[n] = std::ranges::max_element(ftt[n]) - std::ranges::begin(ftt[n]) + 'a';
		m2[n] = std::ranges::min_element(ftt[n]) - std::ranges::begin(ftt[n]) + 'a';
	}
	return std::make_pair(m1, m2);
}

int pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

int main()
{
	auto in = get_input();
	auto[ p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<unsigned, unsigned>> v;
	std::string ln;
	int t = 1;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, th, i] = ctre::match<"Disc #\\d has (\\d+) positions; at time=0, it is at position (\\d+).">(ln); m)
		{
			auto teeth = th.to_number<unsigned>();
			auto init = i.to_number<unsigned>();
			init += t;
			init %= teeth;
			init = teeth - init;
			init %= teeth;
			v.emplace_back(teeth, init);
		}
		else
			fmt::println("parse fail at {}", ln);
		++t;
	}
	std::ranges::sort(v, [](auto& l, auto& r){ return l.first > r.first;});
	return v;
}

int pt12(auto const& in)
{
	timer t("p12");
	int time = in[0].second;
	while(1)
	{
		if(std::ranges::all_of(in, [=](auto& p){ return (time % p.first) == p.second;}))
			break;
		time += in[0].first;
	}
	return time;
}

int main()
{
	auto in = get_input();
	auto p1 = pt12(in);
	unsigned d11 = (11 - in.size() - 1) % 11;
	in.emplace_back(11, d11);
	auto p2 = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

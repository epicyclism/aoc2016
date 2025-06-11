#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

struct cmd
{
	int turn_;
	int steps_;
};

auto get_input()
{
	std::vector<cmd> v;
	std::string ln;
	std::getline(std::cin, ln);
	for(auto[m, t, d]: ctre::search_all<"([LR])(\\d+)">(ln))
		v.emplace_back(t.view()[0] == 'L' ? -1 : 1, d.to_number<int>());
	return v;
}

int new_dir(int t, int cur_dir)
{
	cur_dir += t;
	if(cur_dir < 0)
		cur_dir += 4;
	if(cur_dir > 3)
		cur_dir -= 4;
	return cur_dir;
}

int pt1(auto const& in)
{
	timer t("p1");
	int x = 0;
	int y = 0;
	int d = 0;
	for(auto& c: in)
	{
		d = new_dir(d, c.turn_);
		switch(d)
		{
			case 0:
				y += c.steps_;
				break;
			case 1:
				x += c.steps_;
				break;
			case 2:
				y -= c.steps_;
				break;
			case 3:
				x -= c.steps_;
				break;
			default:
				fmt::println("tilt");
				break;
		}
	}
	return std::abs(x) + std::abs(y);
}

int pt2(auto const& in)
{
	timer t("p2");
	int x = 0;
	int y = 0;
	int d = 0;
	std::set<std::pair<int, int>> cache;
	cache.insert({0,0});
	for(auto& c: in)
	{
		d = new_dir(d, c.turn_);
		switch(d)
		{
			case 0:
				for(int n = 0; n < c.steps_; ++n)
				{
					++y;
					if( cache.contains({x, y}))
						goto figure_it_out;
					cache.insert({x, y});
				}
				break;
			case 1:
				for(int n = 0; n < c.steps_; ++n)
				{
					++x;
					if( cache.contains({x, y}))
						goto figure_it_out;
					cache.insert({x, y});
				}
				break;
			case 2:
				for(int n = 0; n < c.steps_; ++n)
				{
					--y;
					if( cache.contains({x, y}))
						goto figure_it_out;
					cache.insert({x, y});
				}
				break;
			case 3:
				for(int n = 0; n < c.steps_; ++n)
				{
					--x;
					if( cache.contains({x, y}))
						goto figure_it_out;
					cache.insert({x, y});
				}
				break;
			default:
				fmt::println("tilt");
				break;
		}
	}
figure_it_out:
	return std::abs(x) + std::abs(y);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
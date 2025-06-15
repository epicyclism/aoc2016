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

enum class cmd_v {set, r_row, r_col};

struct cmd_t
{
	cmd_v cmd_;
	int x_;
	int y_;
};

auto get_input()
{
	std::vector<cmd_t> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, x, y] = ctre::match<"rect (\\d+)x(\\d+)">(ln); m)
			v.emplace_back(cmd_v::set, x.to_number<int>(), y.to_number<int>());
		else
		if(auto[m, x, y] = ctre::match<"rotate row y=(\\d+) by (\\d+)">(ln); m)
			v.emplace_back(cmd_v::r_row, x.to_number<int>(), y.to_number<int>());
		else
		if(auto[m, x, y] = ctre::match<"rotate column x=(\\d+) by (\\d+)">(ln); m)
			v.emplace_back(cmd_v::r_col, x.to_number<int>(), y.to_number<int>());
		else
			fmt::println("parse fail : {}", ln);
	}
	return v;
}

constexpr int WD = 50;
constexpr int HT = 6; 

void print(std::array<char, WD*HT> const& scr)
{
	for(auto r = 0; r < HT; ++r)
	{
		std::string_view v(scr.begin() + r * WD, scr.begin() + (r + 1) * WD);
		fmt::println("{}", v);
	}
}

void proc(std::vector<cmd_t> const& v, std::array<char, WD*HT>& scr)
{
	std::array<char, HT> tmp_col;
	auto copy_col_out = [&](int col)
	{
		auto itf = std::ranges::begin(scr) + col;
		auto itt = std::ranges::begin(tmp_col);
		for(int n = 0; n < HT; ++n)
		{
			*itt = *itf;
			++itt;
			itf += WD;
		}
	};
	auto copy_col_in  = [&](int col)
	{
		auto itt = std::ranges::begin(scr) + col;
		auto itf = std::ranges::begin(tmp_col);
		for(int n = 0; n < HT; ++n)
		{
			*itt = *itf;
			itt += WD;
			++itf;
		}
	};
	for(auto& c: v)
	{
		auto itb = std::ranges::begin(scr);
		switch(c.cmd_)
		{
			case cmd_v::set:
				for(int y = 0; y < c.y_; ++y)
				{
					for(auto itw = itb; itw != itb + c.x_; ++itw)
						*itw = '#';
					itb += WD;
				}
				break;
			case cmd_v::r_row:
				itb += c.x_ * WD;
				std::ranges::rotate(itb, itb + (WD - c.y_), itb + WD );
				break;
			case cmd_v::r_col:
				copy_col_out(c.x_);
				std::ranges::rotate(std::ranges::begin(tmp_col), std::ranges::begin(tmp_col) + (HT - c.y_), std::ranges::end(tmp_col) );
				copy_col_in(c.x_);
				break;
		}
	}
}

auto pt12(auto const& in)
{
	timer t("p12");
	std::array<char, WD * HT> scr;
	scr.fill('.');
	proc(in, scr);
	return std::make_pair(std::ranges::count(scr, '#'), scr);
}

int main()
{
	auto in = get_input();
	auto[ p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	print(p2);
}

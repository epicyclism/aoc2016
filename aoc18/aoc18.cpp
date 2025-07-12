#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

std::string next_row(std::string_view rw)
{
	std::string r;
	r.resize(rw.size());
	char lft = '.';
	char rgt = '.';
	for(int n = 0; n < rw.size(); ++n)
	{
		if( n > 0)
			lft = rw[n - 1];
		if( n < rw.size() - 1)
			rgt = rw[n + 1];
		else
			rgt = '.';
		auto cntr = rw[n];
		if(lft == '^' && rgt == '.')
			r[n] = '^';
		else
		if(lft == '.' && rgt == '^')
			r[n] = '^';
		else
			r[n] = '.';
	}
	return r;
}

int pt1(auto const& in)
{
	timer t("p1");
	auto s = in;
	int cnt = 0;
	int lim = in.size() < 40 ? in.size() : 40;
	for(int n = 0; n < lim; ++n)
	{
		cnt += std::ranges::count(s.begin(), s.end(), '.');
//		fmt::println("{}", s);
		s = next_row(s);
	}
	return cnt;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	auto s = in;
	int cnt = 0;
	for(int n = 0; n < 400000; ++n)
	{
		cnt += std::ranges::count(s.begin(), s.end(), '.');
		s = next_row(s);
	}
	return cnt;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

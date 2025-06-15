#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

auto get_input()
{
	std::vector<std::string> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		r.emplace_back(std::move(ln));
	return r;
}

bool has_abba(std::string_view v)
{
	if(v.size() < 4)
		return false;
	for(int m = 0; m < v.size() - 3; ++m)
		if(v[m] == v[m + 3] && v[m+1] == v[m+2] && v[m] != v[m+1])
			return true;
	return false;
}

int pt1(auto const& in)
{
	timer t("p1");
	int cnt = 0;
	for(auto& s: in)
	{
		bool has = false;
		bool nothas = false;
		for(auto[m, b]: ctre::search_all<"(\\[[a-z]+\\])">(s))
			if( b && has_abba(b))
				nothas = true;
		if( !nothas)
		{
			for(auto[m, v]: ctre::search_all<"([a-z]+)">(s))
				if(v && has_abba(v))
					has = true;
		}
		cnt += (has && !nothas);
	}
	return cnt;
}

void find_aba(std::string_view v, std::vector<std::pair<char, char>>& lst)
{
	if(v.size() < 3)
		return;
	for(int n = 0; n < v.size() - 2; ++n)
		if(v[n] == v[n + 2] && v[n] != v[n + 1])
			lst.emplace_back(v[n], v[n + 1]);
}

// because RE not great idea the 'aba' vector is a superset of the bab, so undo effect
void tidy_aba(std::vector<std::pair<char, char>>& vaba, std::vector<std::pair<char, char>> const& vbab)
{
	for(auto& bab: vbab)
	{
		if(auto itr = std::ranges::find(vaba, bab); itr != std::ranges::end(vaba))
			vaba.erase(itr);
	}
}

int pt2(auto const& in)
{
	timer t("p2");
	int cnt = 0;
	for(auto& s: in)
	{
		std::vector<std::pair<char, char>> vaba;
		std::vector<std::pair<char, char>> vbab;
		for(auto[m, b]: ctre::search_all<"(\\[[a-z]+\\])">(s))
			find_aba(b, vbab);
		for(auto[m, v]: ctre::search_all<"([a-z]+)">(s))
			find_aba(v, vaba);
		tidy_aba(vaba, vbab);
		for(auto& aba: vaba)
		{
			if(std::ranges::find_if(vbab, [&](auto& bab){ return aba.first == bab.second && aba.second == bab.first;}) != std::ranges::end(vbab))
			{
				++cnt;
				break;
			}
		}
	}

	return cnt;
}
// 368  too high

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

struct room_t
{
	std::string nm_;
	int id_;
	std::string cs_;
};

auto get_input()
{
	std::vector<room_t> r;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, n, i, c] = ctre::match<"([a-z\\-]+)(\\d+)\\[([a-z]+)\\]">(ln); m)
			r.emplace_back(n.str(), i.to_number<int>(), c.str());
	}
	return r;
}

bool good_room(room_t const& r)
{
	std::array<std::pair<int, char>, 26> cnts;
	auto ch = 'a';
	for(auto& p: cnts)
	{
		p.first = 0;
		p.second = ch;
		++ch;
	}
	for(auto c: r.nm_)
		if(c != '-')
			++(cnts[c - 'a'].first);
	std::ranges::sort(cnts, [](auto const& l, auto const& r){if(l.first == r.first) return l.second < r.second; return l.first > r.first;});
	return std::ranges::equal(r.cs_.begin(), r.cs_.end(), cnts.begin(), cnts.begin() + r.cs_.size(),
		[](auto l, auto r){ return l == r.second;});
}

int pt1(auto const& in)
{
	timer t("p1");
	return std::ranges::fold_left(in, 0, [](auto s, auto& r){ return good_room(r) ? s + r.id_ : s;});
}

void decrypt(room_t& r)
{
	int advance = r.id_;
	for(auto& c: r.nm_)
	{
		if(c == '-')
			c = ' ';
		else
			c = (((c - 'a') + advance) % 26) + 'a';
	}
}

int pt2(auto& in)
{
	timer t("p2");
	for(auto& r: in)
		if(good_room(r))
		{
			decrypt(r);
			if(r.nm_.find("north") != std::string::npos)
				return r.id_;
		}
	return 0;
}

auto pt12(auto& in)
{
	timer t("pt12");
	int p1 = 0;
	int p2 = 0;
	for(auto& r: in)
		if(good_room(r))
		{
			p1 += r.id_;
			decrypt(r);
			if(r.nm_.find("north") != std::string::npos)
				p2 = r.id_;
		}
	
	return std::make_pair(p1, p2);
}

int main()
{
	auto in = get_input();
#if 0
	auto p1 = pt1(in);
	auto p2 = pt2(in);
#else
	auto[p1, p2] = pt12(in);
#endif
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

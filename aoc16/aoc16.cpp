#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

std::string dragon(std::string const& a)
{
	std::string b (a);
	std::ranges::reverse(b);;
	std::ranges::transform(b, std::ranges::begin(b), [](auto c){ return c == '0' ? '1' : '0';});
	return a + '0' + b;
}

std::string chksm(std::string const& s)
{
	std::string tmp ;
	std::string cs(s);
	while(!(cs.size() & 1))
	{
		for(int i = 0; i < cs.size(); i +=2)
			tmp.push_back(cs[i] == cs[i+1] ? '1':'0');
		cs.swap(tmp);
		tmp.clear();
	}
	return cs;
}

constexpr auto lent = 20;
constexpr auto len1 = 272;
constexpr auto len2 = 35651584;

auto pt1(auto const& in)
{
	timer t("p1");
	std::string fill = in;
	while(fill.size() < len1)
		fill = dragon(fill);
	fill.erase(len1);

	return chksm(fill);
}

auto pt2(auto const& in)
{
	timer t("p2");
	std::string fill = in;
	while(fill.size() < len2)
		fill = dragon(fill);
	fill.erase(len2);

	return chksm(fill);
}

int main(int ac, char **av)
{
	std::string in = "10010000000110000";
	if(ac > 1)
		in = av[1];
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

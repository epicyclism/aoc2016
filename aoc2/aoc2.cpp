#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::vector<std::string> r;
	std::string ln;
	while(std::getline(std::cin, ln))
		r.emplace_back(std::move(ln));
	return r;
}

struct nxt_t
{
	uint8_t l_;
	uint8_t r_;
	uint8_t u_;
	uint8_t d_;
};

constexpr nxt_t nxt1[] = {   {0, 0, 0, 0}, 
							{1, 2, 1, 4}, {1, 3, 2, 5}, {2, 3, 3, 6}, 
							{4, 5, 1, 7}, {4, 6, 2, 8}, {5, 6, 3, 9},
							{7, 8, 4, 7}, {7, 9, 5, 8}, {8, 9, 6, 9}};

uint8_t code_from_dir(nxt_t const& n, char d)
{
	if(d == 'L')
		return n.l_;
	if(d == 'R')
		return n.r_;
	if(d == 'U')
		return n.u_;
	return n.d_;
}

std::string pt1(auto const& in)
{
	timer t("p1");
	uint8_t b = 5;
	std::string r;
	for(auto s: in)
	{
		for(auto c: s)
			b = code_from_dir(nxt1[b], c);
		r.push_back(b + '0');
	}

	return r;
}

constexpr nxt_t nxt2[] = {  {0, 0, 0, 0},
												    {1, 1, 1, 3},
									  {2, 3, 2, 6}, {2, 4, 1, 7}, {3, 4, 4, 8},
						{5, 6, 5, 5}, {5, 7, 3, 10}, {6, 8, 3, 11}, {7, 9, 4, 12}, {8, 9, 9, 9},
									  {10, 11, 6, 10}, {10, 12, 7, 13}, {11, 12, 8, 12},
													{13, 13, 11, 13}};

constexpr char os[] = "0123456789ABCD";

std::string pt2(auto const& in)
{
	timer t("p2");
	uint8_t b = 5;
	std::string r;
	for(auto s: in)
	{
		for(auto c: s)
			b = code_from_dir(nxt2[b], c);
		r.push_back(os[b]);
	}
	return r;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

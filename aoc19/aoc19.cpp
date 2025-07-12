#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

int next(std::vector<int> const& v, int p)
{
	int n = p;
	while( n < v.size())
	{
		if(v[n])
			return n;
		++n;
	}
	n = 0;
	while(n < p)
	{
		if(v[n])
			return n;
		++n;
	}
	return n;
}

void print(std::vector<int> const& v)
{
	for(auto i: v)
		fmt::print("{} ", i);
	fmt::println("");
}

int pt1(auto in)
{
	timer t("p1");
	std::vector<int> v(in, 1);
	int p = 0;
	int n = next(v, p + 1);
	while( n != p)
	{
		v[p] += v[n];
		v[n] = 0;
		p = next(v, p + 1);
		n = next(v, p + 1);
	}
	return n + 1;
}

// 31728 too low

int pt2(auto in)
{
	timer t("p2");
	std::vector<std::pair<int, int>> v(in);
	for(int i = 0; i < v.size(); ++i)
	{
		v[i].first = 1;
		v[i].second = i + 1;
	}
	int e = 0;
	while(v.size() > 1)
	{
		int o = e + v.size() / 2;
		if(o >= v.size())
			o -= v.size();
		v[e].first += v[o].first;
		v.erase(v.begin()+o);
		if( o > e)
			++e;
		if( e >= v.size())
			e = 0;
	}
	return v[0].second;
}

int main(int ac, char** av)
{
	auto in = 3014387;
	if( ac > 1)
		in = sv_to_t<int>(av[1]);
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "ctre_inc.h"
#include "timer.h"

int pt1(auto in)
{
	timer t("p1");
	std::deque<int> d(in);
	std::iota(d.begin(), d.end(), 1);
	while(d.size() > 1)
	{
		d.push_back(d.front());
		d.pop_front();
		d.pop_front();
	}
	return d.front();
}

int pt2(auto in)
{
	timer t("p2");
    int p = 2;

    for(int i = 4; i <= in; ++i)
	{
	    ++p;
	    p %= (i - 1);
	    if(p >= i / 2)
			++p;
    }

    return p + 1; //zero based
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

#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

int pt1(auto in)
{
	timer t("p1");
	return 0;
}

int pt2(auto in)
{
	timer t("p2");
	return 0;
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

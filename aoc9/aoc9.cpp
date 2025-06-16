#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

size_t decompress(std::string_view in)
{
	size_t sz = 0;
	while(1)
	{
		if(auto[mtch, len, cnt] = ctre::search<"\\((\\d+)x(\\d+)\\)">(in); mtch)
		{
			auto tmp = mtch.view().begin() - in.begin();
			sz += tmp;
			in.remove_prefix(tmp);
			int chars = len.to_number<int>();
			int times = cnt.to_number<int>();
			sz += chars * times;
			in.remove_prefix(mtch.view().size() + chars);
		}
		else
			break;
	}
	sz += in.size();
	return sz;
}

size_t decompress2(std::string_view in)
{
	size_t sz = 0;
	while(1)
	{
		if(auto[mtch, len, cnt] = ctre::search<"\\((\\d+)x(\\d+)\\)">(in); mtch)
		{
			auto tmp = mtch.view().begin() - in.begin();
			sz += tmp;
			in.remove_prefix(tmp);
			int chars = len.to_number<int>();
			int times = cnt.to_number<int>();
			std::string_view vt(mtch.view().end(), mtch.view().end() + chars);
			sz += decompress2(vt) * times;
			in.remove_prefix(mtch.view().size() + chars);
		}
		else
			break;
	}
	sz += in.size();
	return sz;
}

auto pt1(auto const& in)
{
	timer t("p1");
	return decompress(in);
}

auto pt2(auto const& in)
{
	timer t("p2");
	return decompress2(in);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

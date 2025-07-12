#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<uint32_t, uint32_t>> fw;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, l, h] = ctre::match<"(\\d+)-(\\d+)">(ln); m)
			fw.emplace_back(l.to_number<uint32_t>(), h.to_number<uint32_t>());
		else
			fmt::println("parse fail {}", ln);
	}
	timer t("p12 preprocess");
	std::ranges::sort(fw, [](auto& l, auto& r){return l.first < r.first;});
	bool condense = true;
	while(condense)
	{
		condense = false;
		for(int p = 0; p < fw.size() - 1; ++p)
		{
			if( fw[p].second >= fw[p+1].first - 1)
			{
				condense = true;
				fw[p].second = std::max(fw[p].second, fw[p+1].second);
				fw.erase(fw.begin() + p + 1);
				goto again;
			}
		}
again:
	}
	return fw;
}

uint32_t pt1(auto const& in)
{
	return in[0].second + 1;
}

uint32_t pt2(auto const& in)
{
	uint32_t cnt = 0;
	for(int n = 0; n < in.size() - 1; ++n)
		cnt += in[n + 1].first - in[n].second - 1;
	cnt += std::numeric_limits<uint32_t>::max() - in.back().second;
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

#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "graph.h"
#include "timer.h"

struct maze
{
	std::vector<char> gd_;
	size_t st_;
	std::vector<size_t>  pts_;
};

auto get_input()
{
	maze g;
	std::string ln;
	std::getline(std::cin, ln);
	g.st_ = ln.size();
	auto pt = ln.size();
	g.gd_.append_range(ln);
	while(std::getline(std::cin, ln))
	{
		for(int n = 0; n < ln.size(); ++n)
			if(::isdigit(ln[n]))
			{
				if(ln[n] == '0')
					g.pts_.insert(g.pts_.begin(), n + pt);
				else
					g.pts_.emplace_back(n + pt);
			}
		g.gd_.append_range(ln);
		pt += ln.size();
	}
	return g;
}

int add_flag(int flgs, int f)
{
	return flgs | (1 << f);
}

int is_flag(int flgs, int f)
{
	return flgs & (1 << f);
}

int all_flags(int nf)
{
	int rv = 0;
	while(nf > 0)
	{
		rv <<= 1;
		rv |= 1;
		--nf;
	}
	return rv;
}

void pt12_wkr( auto& g, auto& al, int v, int flgs, int all_flgs, int tot, int& mn1, int& mn2)
{
	if(flgs == all_flgs)
	{
		if(tot < mn1)
			mn1 = tot;
		if(tot + al[v][0] < mn2)
			mn2 = tot + al[v][0];
		return;
	}
	if(tot > mn1)
		return;
	for(int vn = 0; vn < al[v].size(); ++vn)
	{
		if(!is_flag(flgs, vn))
			pt12_wkr(g, al, vn, add_flag(flgs, vn), all_flgs, tot + al[v][vn], mn1, mn2);
	}
}

auto pt12(auto const& in)
{
	timer t("p1");
	std::vector<std::vector<int>> al(in.pts_.size());
	grid_direct_border g (in.gd_, in.st_, [&](auto t){return in.gd_[t] != '#';}); 
	for(int spv = 0; spv < in.pts_.size(); ++spv)
	{
		auto d = bfs(g, in.pts_[spv]);
		int pv = 0;
		for(int pv = 0; pv < in.pts_.size(); ++pv)
			al[spv].emplace_back(d[in.pts_[pv]]);
	}
	int mn1 = std::numeric_limits<int>::max();
	int mn2 = std::numeric_limits<int>::max();
	pt12_wkr(g, al, 0, 1, all_flags(in.pts_.size()), 0, mn1, mn2);
	return std::make_pair(mn1, mn2);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

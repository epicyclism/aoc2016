#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

struct node
{
	int x_;
	int y_;
	int sz_;
	int used_;
};

struct node_r
{
	int sz_;
	int used_;
};

auto get_input()
{
	std::vector<node> vn;
	std::string ln;
	while(std::getline(std::cin, ln))
		if(auto[m, x, y, s, u] = ctre::match<"/dev/grid/node-x(\\d+)-y(\\d+)\\s+(\\d+)T\\s+(\\d+)T\\s+\\d+T\\s+\\d+%">(ln); m)
			vn.emplace_back(x.to_number<int>(), y.to_number<int>(), s.to_number<int>(), u.to_number<int>());
	std::ranges::sort(vn, [](auto& l, auto& r){if(l.y_ == r.y_) return l.x_ < r.x_; return l.y_ < r.y_;});
	auto mx = std::ranges::max_element(vn, std::ranges::less(), &node::x_)->x_;
	std::vector<node_r> vnr(vn.size());
	std::ranges::transform(vn, std::ranges::begin(vnr), [](auto& n){ return node_r(n.sz_, n.used_);});
	return std::make_pair(vnr, mx + 1);
}

int pt1(auto const& in)
{
	timer t("p1");
	int cnt = 0;
	for(int a = 0; a < in.size(); ++a)
		for(int b = a + 1; b < in.size(); ++b)
			cnt += (in[a].used_ && (in[b].sz_ - in[b].used_ >= in[a].used_)) ||
				(in[b].used_ && (in[a].sz_ - in[a].used_ >= in[b].used_));

	return cnt;
}

std::array<int, 4> adjacent(int stride, int size, int pos)
{
	std::array<int, 4> r;
	r.fill(-1);
	if(pos % stride > 0)
		r[0] = pos - 1;
	if(pos % stride < stride - 1)
		r[1] = pos + 1;
	if( pos >= stride)
		r[2] = pos - stride;
	if( pos < size - stride - 1)
		r[3] = pos + stride;
	return r;
}

void step(std::vector<node_r> v, std::vector<bool> visited, int stride, int pos, int steps, int& mn)
{
	fmt::println("step({}, {})", pos, steps);
	if(steps > mn)
		return;
	if(pos == 0)
	{
		if( steps < mn)
			mn = steps;
		return;
	}
	auto a = adjacent(stride, v.size(), pos);
	for(auto np: a)
	{
		if( np != -1 && !visited[np])
		{
			fmt::println("{} -> {} ({})", pos, np, v[np].sz_ - v[np].used_);
			if(v[np].sz_ - v[np].used_ >= v[pos].used_)
			{
				fmt::println("{} -> {}", pos, np);
				auto u = v[pos].used_;
				v[np].used_ += u;
				v[pos].used_ = 0;
				visited[np] = true;
				step(v, visited, stride, np, steps + 1, mn );
				visited[np] = false;
				v[pos].used_ = u;
				v[np].used_ -= u;
			}
			else
			{
				// look for somewhere to move np's load to
				auto a2 = adjacent(stride, v.size(), np);
				for(auto np2: a2)
				{
					if( np2 == -1 || np2 == pos)
						continue;
					fmt::println("- {} -> {} ({})", np, np2, v[np2].sz_ - v[np2].used_);
					if(v[np2].sz_ - v[np2].used_ >= v[np].used_)
					{
						auto u = v[np].used_;
						v[np2].used_ += u;
						v[np].used_ = 0;
						step(v, visited, stride, pos, steps + 1, mn );
						v[np2].used_ -= u;
						v[np].used_ = u;		
					}
				}
			}
		}
	}
}

int pt2(auto const& vn, int stride)
{
	timer t("p2");
	fmt::println("pt2({}, {})", vn.size(), stride);
	int mn = std::numeric_limits<int>::max();
	std::vector<bool> visited(vn.size());
	visited[stride - 1] = true;
	step(vn, visited, stride, stride - 1, 0, mn);
	return mn;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in.first);
	auto p2 = pt2(in.first, in.second);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

#include <iostream>
#include <string>
#include <vector>
#include <queue>
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
	if( pos < size - stride)
		r[3] = pos + stride;
	return r;
}

// find the nearest node with space
// return the number of steps and the node id.
// id_not is a node id not to look at or pass through.
auto bfs(auto const& vn, int stride, int id_from, int id_not, int amt) 
{
	std::vector<int>	distance(vn.size(), 0);
    std::vector<bool>   visited(vn.size());
	visited[id_not] = true;
    std::queue<int> q;
    q.push(id_from);
    visited[id_from] = true;
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        for (auto e : adjacent(stride, vn.size(), u))
        {
            if (e != -1 && !visited[e])
            {
                visited[e] = true;
				if(vn[e].used_ < 100) // arbitrary for the proper inputs to avoid considering nodes we cannot move from.
				{
					distance[e] = distance[u] + 1;
					if( vn[e].sz_ - vn[e].used_ >= amt)
						return std::make_pair(e, distance[e]);
					q.push(e);
				}
            }
        }
    }
    return std::make_pair(-1, -1);
} 

int pt2(auto const& vn0, int stride)
{
	timer t("p2");
	auto vn = vn0;
	int steps = 0;

	for(int x = stride - 2; x >= 0; --x)
	{
		++steps;
		auto[n, s] = bfs(vn, stride, x, x + 1, vn[x + 1].used_);
		steps += s;
		vn[n].used_ = vn[x].used_;
		vn[x].used_ = vn[x + 1].used_;
		vn[x + 1].used_ = 0;
	}
	return steps;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in.first);
	auto p2 = pt2(in.first, in.second);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

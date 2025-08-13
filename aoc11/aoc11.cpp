#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"
#include "linear_set.h"
#include "graph.h"

struct state
{
	linear_set<std::string> names_;

	std::vector<uint8_t> generators_;
	std::vector<uint8_t> microchips_;
	uint8_t elevator_;
	uint32_t encode() const noexcept
	{
		uint32_t u{ 0 };
		uint32_t shft = 0;
		for (int e = 0; e < generators_.size(); ++e)
		{
			u |= (uint32_t(microchips_[e]) << shft);
			shft += 2;
			u |= (uint32_t(generators_[e]) << shft);
			shft += 2;
		}
		u |= (uint32_t(elevator_)) << shft;
		return u;
	}
	void decode(uint32_t u)
	{
		uint32_t msk = 0x00000003;
		uint32_t shft = 0;
		for (int e = 0; e < generators_.size(); ++e) // assume decoding into an existing valid state
		{
			microchips_[e] = (u & msk) >> shft;
			msk <<= 2;
			shft += 2;
			generators_[e] = (u & msk) >> shft;
			msk <<= 2;
			shft += 2;
		}
		elevator_ = (u & msk) >> shft;
	}
	void set_to_end()
	{
		for(auto& g: generators_)
			g = 3;
		for(auto& m: microchips_)
			m = 3;
		elevator_ = 3;
	}
	void add_generator(std::string_view name, uint8_t pos)
	{
		auto p = names_.add(name);
		if(generators_.size() < p + 1)
			generators_.resize(p + 1);
		generators_[p] = pos;
	}
	void add_microchip(std::string_view name, uint8_t pos)
	{
		auto p = names_.add(name);
		if(microchips_.size() < p + 1)
			microchips_.resize(p + 1);
		microchips_[p] = pos;
	}
	bool is_valid() const noexcept
	{
		// for each floor, state is invalid if there is any generator and an unmatched microchip
		for (int f = 0; f < 4; ++f)
		{
			bool unmatched_m = false;
			bool any_g = false;
			for (int e = 0; e < generators_.size(); ++e)
			{
				if (microchips_[e] == f && !(generators_[e] == f))
					unmatched_m = true;
				if (generators_[e] == f)
					any_g = true;
			}
			if (unmatched_m && any_g)
				return false;
		}
		return true;
	}
	uint8_t& gm(int e)
	{
		if (e < generators_.size())
			return generators_[e];
		else
			return microchips_[e - generators_.size()];
	}
	std::vector<uint32_t> next_moves()
	{
		std::vector<uint32_t> rv;
		auto el = elevator_;
		if (el < 3) // can go up!
		{
			++elevator_;
			// move one
			for (int e = 0; e < 2 * generators_.size(); ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					++re;
					if (is_valid())
						rv.emplace_back(encode());
					--re;
				}
			}
			// move two
			for (int e = 0; e < 2 * generators_.size() - 1; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					++re;
					for (int e2 = e + 1; e2 < 2 * generators_.size(); ++e2)
					{
						auto& re2 = gm(e2);
						if (re2 == el)
						{
							++re2;
							if (is_valid())
								rv.emplace_back(encode());
							--re2;
						}
					}
					--re;
				}
			}
			--elevator_;
		}
		if (el > 0) // can go down!
		{
			--elevator_;
			// move one
			for (int e = 0; e < 2 * generators_.size(); ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					--re;
					if (is_valid())
						rv.emplace_back(encode());
					++re;
				}
			}
			// move two
			for (int e = 0; e < 2 * generators_.size() - 1; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					--re;
					for (int e2 = e + 1; e2 < 2 * generators_.size(); ++e2)
					{
						auto& re2 = gm(e2);
						if (re2 == el)
						{
							--re2;
							if (is_valid())
								rv.emplace_back(encode());
							++re2;
						}
					}
					++re;
				}
			}
			++elevator_;
		}
		return rv;
	}
	uint32_t max_state() const
	{
		auto r = 1 << (2 * generators_.size() + 1) * 2;
		return r;
	}
};

auto get_input()
{
	state st;
	std::string ln;
	uint8_t pos = 0;
	while(std::getline(std::cin, ln))
	{
		// generators
		for(auto[m, n]: ctre::search_all<"([a-z]+) generator">(ln))
			st.add_generator(n.view(), pos);
		// microchips
		for(auto[m, n]: ctre::search_all<"([a-z]+)-compatible">(ln))
			st.add_microchip(n.view(), pos);
		++pos;
	}
	st.elevator_ = 0;

//	for(auto& n: st.names_)
//		fmt::println("{}", n);
//	fmt::println("generators {}, microchips {}", st.generators_.size(), st.microchips_.size());
	return st;
}

using graph_t = std::vector<std::vector<uint32_t>>;

void build_graph2(graph_t& gt, state& st)
{
	timer t("build_graph");
	for (uint32_t u = 0; u < st.max_state(); ++u)
	{
		st.decode(u);
		auto vm = st.next_moves();
		for (auto ue : vm)
		{
			add_edge(gt, u, ue);
		}
	}
}

int64_t pt1a(auto in)
{
	timer t("p1");
	graph_t g;
	auto f = in.encode();
	build_graph2(g, in);
	in.set_to_end();
	auto e = in.encode();
	fmt::println("vertex count - {}", g.size());
	auto dists = bfs(g, f);
	return dists.at(e);
}

int bfs_act(auto& s, auto f, auto e)
{
	std::queue<std::pair<uint32_t, int>> q;
	std::vector<bool> visited(s.max_state());
	visited[f] = true;
	q.push(std::make_pair(f, 0));
	while(!q.empty())
	{
		auto n = q.front();
		q.pop();
		if(n.first == e)
			return n.second;
		s.decode(n.first);
		auto vm = s.next_moves();
		for (auto ue : vm)
		{
			if(!visited[ue])
			{
				visited[ue] = true;
				q.push(std::make_pair(ue, n.second + 1));
			}
		}
	}
	return -1;
}

int pt1(auto in)
{
	timer t("p1");
	auto f = in.encode();
	in.set_to_end();
	auto e = in.encode();
	return bfs_act(in, f, e);
}

int64_t pt2a(auto in)
{
	timer t("p2");
	in.add_generator("dilithium", 0);
	in.add_microchip("dilithium", 0);
	in.add_generator("elerium", 0);
	in.add_microchip("elerium", 0);
	graph_t g;
	auto f = in.encode();
	build_graph2(g, in);
	in.set_to_end();
	auto e = in.encode();
	fmt::println("vertex count - {}", g.size());
	auto dists = bfs(g, f);
	return dists.at(e);
}

int pt2(auto in)
{
	timer t("p2");
	in.add_generator("dilithium", 0);
	in.add_microchip("dilithium", 0);
	in.add_generator("elerium", 0);
	in.add_microchip("elerium", 0);
	auto f = in.encode();
	in.set_to_end();
	auto e = in.encode();
	return bfs_act(in, f, e);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

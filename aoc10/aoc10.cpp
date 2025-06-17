#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

struct bot
{
	int low_;
	int high_;
	int chipA_ = -1;
	int chipB_ = -1;
	void set(int v)
	{
		if(chipA_ == -1)
			chipA_ = v;
		else
			chipB_ = v;
	}
	void sort()
	{
		if( chipB_ < chipA_)
			std::swap(chipA_, chipB_);
	}
};

using bots_t = std::vector<bot>;

template<typename T> T& get_T(std::vector<T>& b, int n)
{
	if(b.size() < n + 1)
		b.resize(n + 1);
	return b[n];
}

auto get_input()
{
	bots_t bots;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, v, b] = ctre::match<"value (\\d+) goes to bot (\\d+)">(ln); m)
		{
			auto& bt = get_T(bots, b.to_number<int>());	
			bt.set(v.to_number<int>());
		}
		else
		if(auto[m, b, lbo, l, hbo, h] = ctre::match<"bot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)">(ln); m)
		{
			auto& bt = get_T(bots, b.to_number<int>());	
			bt.low_ = l.to_number<int>();
			if(lbo.view()[0] != 'b')
			{
				bt.low_ *= -1;
				--bt.low_ ;
			}
			bt.high_ = h.to_number<int>();
			if(hbo.view()[0] != 'b')
			{
				bt.high_ *= -1;
				--bt.high_;
			}
		}
		else
			fmt::println("parse fail : {}", ln);
	}
	return bots;
}

std::vector<int> topo_sort(bots_t const& b)
{
	std::vector<int> o;
	o.reserve(b.size());
	std::vector<uint8_t> flgs (b.size(), 1);
	std::ranges::transform(b, flgs.begin(), [](auto const& g)
		{
			uint8_t u = 1;
			if(g.chipA_ >= 0)
				++u;
			if(g.chipB_ >= 0)
				++u;
			return u;
		});
	int cnt = 1;
	while(cnt)
	{
		cnt = 0;
		for(int n = 0; n < b.size(); ++n)
		{
			if(flgs[n] == 0x03) // inputs satisfied
			{
				o.push_back(n);
				flgs[n] = 0;
				++flgs[b[n].low_];
				++flgs[b[n].high_];
				++cnt;
			}
		}
	}
	return o;
}

std::vector<int> execute(bots_t& b, std::vector<int> const& order)
{
	std::vector<int> outs;
	for(auto k : order)
	{
		b[k].sort();
		if(b[k].low_ < 0)
		{
			int& on = get_T(outs, b[k].low_ * -1);
			on = b[k].chipA_;
		}
		else
		{
			b[b[k].low_].set(b[k].chipA_);
		}
		if(b[k].high_ < 0)
		{
			int& on = get_T(outs, b[k].high_ * -1);
			on = b[k].chipB_;
		}
		else
		{
			b[b[k].high_].set(b[k].chipB_);
		}
	}
	return outs;
}

auto pt12(auto& in)
{
	timer t("p1");
	auto order = topo_sort(in);
	auto outs = execute(in, order);
	for(int n = 0; n < in.size(); ++n)
		if( in[n].chipA_ == 17 && in[n].chipB_ == 61)
			return std::make_pair(n, outs[1] * outs[2] * outs[3]);

	return std::make_pair(0, 0);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

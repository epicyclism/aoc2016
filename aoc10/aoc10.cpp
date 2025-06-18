#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre.hpp"
#include "timer.h"

const unsigned invalid_chip = unsigned(-1);
const unsigned out_flag = 0x80000000;

bool is_out(unsigned n)
{
	return n & out_flag;
}

unsigned set_out(unsigned n)
{
	return n | 	out_flag;
}

unsigned get_out(unsigned n)
{
	return n & ~out_flag;
}

struct bot
{
	unsigned low_;
	unsigned high_;
	unsigned chipA_ = invalid_chip;
	unsigned chipB_ = invalid_chip;
	void set(int v)
	{
		if(chipA_ == invalid_chip)
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
			bt.set(v.to_number<unsigned>());
		}
		else
		if(auto[m, b, lbo, l, hbo, h] = ctre::match<"bot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)">(ln); m)
		{
			auto& bt = get_T(bots, b.to_number<unsigned>());	
			bt.low_ = l.to_number<unsigned>();
			if(lbo.view()[0] != 'b')
				bt.low_ = set_out(bt.low_);
			bt.high_ = h.to_number<unsigned>();
			if(hbo.view()[0] != 'b')
				bt.high_ = set_out(bt.high_);
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
	std::vector<int> flgs (b.size());
	std::ranges::transform(b, flgs.begin(), [](auto const& g)
		{
			int u = 1;
			if(g.chipA_ != invalid_chip)
				++u;
			if(g.chipB_ != invalid_chip)
				++u;
			return u;
		});
	while(!std::ranges::all_of(flgs, [](auto f){ return f == 0;} ))
	{
		for(int n = 0; n < b.size(); ++n)
		{
			if(flgs[n] == 3) // inputs satisfied
			{
				o.push_back(n);
				flgs[n] = 0;
				if(!is_out(b[n].low_))
					++flgs[b[n].low_];
				if(!is_out(b[n].high_))
					++flgs[b[n].high_];
			}
		}
	}
	return o;
}

auto execute(bots_t& b, std::vector<int> const& order)
{
	std::vector<unsigned> outs;
	for(auto k : order)
	{
		bot& br = b[k];
		br.sort();
		if(is_out(br.low_))
		{
			unsigned& on = get_T(outs, get_out(br.low_));
			on = b[k].chipA_;
		}
		else
			b[b[k].low_].set(b[k].chipA_);
		if(is_out(b[k].high_ ))
		{
			unsigned& on = get_T(outs, get_out(b[k].high_));
			on = b[k].chipB_;
		}
		else
			b[b[k].high_].set(b[k].chipB_);
	}
	return outs;
}

auto pt12(auto& in)
{
	timer t("p12");
	auto order = topo_sort(in);
	auto outs = execute(in, order);
	for(unsigned n = 0; n < in.size(); ++n)
		if( in[n].chipA_ == 17 && in[n].chipB_ == 61)
			return std::make_pair(n, outs[0] * outs[1] * outs[2]);

	return std::make_pair(0U, 0U);
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

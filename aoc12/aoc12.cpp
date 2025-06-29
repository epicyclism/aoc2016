#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class inst_t {cpy, cpyi, inc, dec, jnz, jnzi};

struct inst
{
	inst_t  i_;
	int64_t x_;
	int64_t y_;
};
using regs = std::array<int64_t, 4>;

void proc(regs& rr, std::vector<inst> const& v)
{
	size_t ip = 0;
	while(ip < v.size())
	{
		auto& i = v[ip];
		switch(i.i_)
		{
			case inst_t::cpy:
				rr[i.y_] = rr[i.x_];
				break;
			case inst_t::cpyi:
				rr[i.y_] = i.x_;
				break;
			case inst_t::inc:
				++rr[i.x_];
				break;
			case inst_t::dec:
				--rr[i.x_];
				break;
			case inst_t::jnz:
				if(rr[i.x_] != 0)
					ip += i.y_ - 1;
				break;
			case inst_t::jnzi:
				if(i.x_ != 0)				
					ip += i.y_ - 1;
				break;
			default:
				fmt::println("CANT HAPPEN BAD INSTRUCTION");
				break;
		}
		++ip;
	}
}

int64_t to_reg(std::string_view v)
{
	return v[0] - 'a';
}

auto get_input()
{
	std::vector<inst> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, x, y] = ctre::match<"cpy ([abcd]) ([abcd])">(ln); m)
			rv.emplace_back(inst_t::cpy, to_reg(x), to_reg(y));
		else
		if(auto[m, x, y] = ctre::match<"cpy (\\-?\\d+) ([abcd])">(ln); m)
			rv.emplace_back(inst_t::cpyi, x.to_number<int64_t>(), to_reg(y));
		else
		if(auto[m, x] = ctre::match<"inc ([abcd])">(ln); m)
			rv.emplace_back(inst_t::inc, to_reg(x), 0);
		else
		if(auto[m, x] = ctre::match<"dec ([abcd])">(ln); m)
			rv.emplace_back(inst_t::dec, to_reg(x), 0);
		else
		if(auto[m, x, y] = ctre::match<"jnz ([abcd]) (\\-?\\d+)">(ln); m)
			rv.emplace_back(inst_t::jnz, to_reg(x), y.to_number<int64_t>());
		else
		if(auto[m, x, y] = ctre::match<"jnz (\\-?\\d+) (\\-?\\d+)">(ln); m)
			rv.emplace_back(inst_t::jnzi, x.to_number<int64_t>(), y.to_number<int64_t>());
		else
			fmt::println("bad parse : {}", ln);
	}
	return rv;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	regs rr;
	rr.fill(0);
	proc(rr, in);
	return rr[0];
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	regs rr;
	rr.fill(0);
	rr[2] = 1;
	proc(rr, in);
	return rr[0];
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

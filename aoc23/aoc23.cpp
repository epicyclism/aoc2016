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

enum class inst_t {cpyrr, cpyri, cpyii, cpyir, inc, inci, dec, jnzrr, jnzri, jnzii, jnzir, tgl, tgli};

struct inst
{
	inst_t  i_;
	int64_t x_;
	int64_t y_;
};
using regs = std::array<int64_t, 4>;

void toggle(inst& i)
{
	switch(i.i_)
	{
		case inst_t::cpyrr:
			i.i_ = inst_t::jnzrr;
			break;
		case inst_t::cpyri:
			i.i_ = inst_t::jnzri;
			break;
		case inst_t::cpyii:
			i.i_ = inst_t::jnzii;
			break;
		case inst_t::cpyir:
			i.i_ = inst_t::jnzir;
			break;
		case inst_t::inci:
			i.i_ = inst_t::tgli;
		break;
		case inst_t::inc:
			i.i_ = inst_t::dec;
		break;
		case inst_t::dec:
			i.i_ = inst_t::inc;
		break;
		case inst_t::jnzrr:
			i.i_ = inst_t::cpyrr;
		break;
		case inst_t::jnzri:
			i.i_ = inst_t::cpyri;
		break;
		case inst_t::jnzii:
			i.i_ = inst_t::cpyii;
		break;
		case inst_t::jnzir:
			i.i_ = inst_t::cpyir;
		break;
		case inst_t::tgl:
			i.i_ = inst_t::inc;
		break;
		case inst_t::tgli:
			i.i_ = inst_t::inci;
		break;
	}
}

void proc(regs& rr, std::vector<inst>& v)
{
	size_t ip = 0;
	while(ip < v.size())
	{
		auto& i = v[ip];
		switch(i.i_)
		{
			case inst_t::cpyrr:
				rr[i.y_] = rr[i.x_];
				break;
			case inst_t::cpyri:
				break;
			case inst_t::cpyii:
				break;
			case inst_t::cpyir:
				rr[i.y_] = i.x_;
				break;
			case inst_t::inc:
				++rr[i.x_];
				break;
			case inst_t::inci:
				break;
			case inst_t::dec:
				--rr[i.x_];
				break;
			case inst_t::jnzrr:
				if(rr[i.x_] != 0)
					ip += rr[i.y_] - 1;
				break;
			case inst_t::jnzii:
				if(i.x_ != 0)				
					ip += i.y_ - 1;
				break;
			case inst_t::jnzir:
				if(i.x_ != 0)
					ip += rr[i.y_] - 1;
				break;
			case inst_t::jnzri:
				if(rr[i.x_] != 0)				
					ip += i.y_ - 1;
				break;
			case inst_t::tgl:
				{
					auto dest = ip + rr[i.x_];
					if(dest >= 0 && dest < v.size())
						toggle(v[dest]);
				}
				break;
			case inst_t::tgli:
				{
					auto dest = ip + i.x_;
					if(dest >= 0 && dest < v.size())
						toggle(v[dest]);
				}
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
			rv.emplace_back(inst_t::cpyrr, to_reg(x), to_reg(y));
		else
		if(auto[m, x, y] = ctre::match<"cpy (\\-?\\d+) ([abcd])">(ln); m)
			rv.emplace_back(inst_t::cpyir, x.to_number<int64_t>(), to_reg(y));
		else
		if(auto[m, x] = ctre::match<"inc ([abcd])">(ln); m)
			rv.emplace_back(inst_t::inc, to_reg(x), 0);
		else
		if(auto[m, x] = ctre::match<"dec ([abcd])">(ln); m)
			rv.emplace_back(inst_t::dec, to_reg(x), 0);
		else
		if(auto[m, x, y] = ctre::match<"jnz ([abcd]) (\\-?\\d+)">(ln); m)
			rv.emplace_back(inst_t::jnzri, to_reg(x), y.to_number<int64_t>());
		else
		if(auto[m, x, y] = ctre::match<"jnz ([abcd]) ([abcd])">(ln); m)
			rv.emplace_back(inst_t::jnzrr, to_reg(x), to_reg(y));
		else
		if(auto[m, x, y] = ctre::match<"jnz (\\-?\\d+) (\\-?\\d+)">(ln); m)
			rv.emplace_back(inst_t::jnzii, x.to_number<int64_t>(), y.to_number<int64_t>());
		else
		if(auto[m, x, y] = ctre::match<"jnz (\\-?\\d+) ([abcd])">(ln); m)
			rv.emplace_back(inst_t::jnzir, x.to_number<int64_t>(), to_reg(y));
		else
		if(auto[m, x] = ctre::match<"tgl ([abcd])">(ln); m)
			rv.emplace_back(inst_t::tgl, to_reg(x), 0);
		else
		if(auto[m, x] = ctre::match<"tgl (\\-?\\d+)">(ln); m)
			rv.emplace_back(inst_t::tgli, x.to_number<int64_t>(), 0);
		else
			fmt::println("bad parse : {}", ln);
	}
	return rv;
}

int64_t pt1(auto in)
{
	timer t("p1");
	regs rr;
	rr.fill(0);
	rr[0] = 7;
	proc(rr, in);
	return rr[0];
}

int64_t pt2(auto in)
{
	timer t("p2");
	regs rr;
	rr.fill(0);
	rr[0] = 12;
	proc(rr, in);
	return rr[0];
}

// input2 479007513

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class act { swap_pos, swap_letter, rotate_left, rotate_right,
				 rotate_pos, reverse, move };

struct action
{
	act act_;
	uint8_t a_;
	uint8_t b_;
};

uint8_t do_rotate[]   = {1, 2, 3, 4, 6, 7, 0, 1}; // position to right rotation
uint8_t undo_rotate[] = {1, 1, 6, 2, 7, 3, 0, 4}; // position to left rotation for reversal

void do_act(action const& a, std::string& s)
{
	switch(a.act_)
	{
		case act::swap_pos:
			std::swap(*(s.begin() + a.a_), *(s.begin() + a.b_));
			break;
		case act::swap_letter:
			{
				auto X = s.find(a.a_);
				auto Y = s.find(a.b_);
				std::swap(*(s.begin() + X), *(s.begin() + Y));
			}
			break;
		case act::rotate_left:
			std::rotate(s.begin(), s.begin() + a.a_, s.end());
			break;
		case act::rotate_right:
			std::rotate(s.begin(), s.begin() + s.size() - a.a_, s.end());
			break;
		case act::rotate_pos:
			{
				auto r = s.find(a.a_);
				std::rotate(s.begin(), s.begin() + s.size() - do_rotate[r], s.end());
			}
			break;
		case act::reverse:
			std::reverse(s.begin() + a.a_, s.begin() + a.b_ + 1);
			break;
		case act::move:
			{
				auto f = s[a.a_];
				s.erase(a.a_, 1);
				s.insert(a.b_, 1, f);
			}
			break;
	}
}

void do_ract(action const& a, std::string& s)
{
	switch(a.act_)
	{
		case act::swap_pos:
			std::swap(*(s.begin() + a.a_), *(s.begin() + a.b_));
			break;
		case act::swap_letter:
			{
				auto X = s.find(a.a_);
				auto Y = s.find(a.b_);
				std::swap(*(s.begin() + X), *(s.begin() + Y));
			}
			break;
		case act::rotate_left:
			std::rotate(s.begin(), s.begin() + s.size() - a.a_, s.end());
			break;
		case act::rotate_right:
			std::rotate(s.begin(), s.begin() + a.a_, s.end());
			break;
		case act::rotate_pos:
			{
				auto r = s.find(a.a_);
				std::rotate(s.begin(), s.begin() + undo_rotate[r], s.end());
			}
			break;
		case act::reverse:
			std::reverse(s.begin() + a.a_, s.begin() + a.b_ + 1);
			break;
		case act::move:
			{
				auto f = s[a.b_];
				s.erase(a.b_, 1);
				s.insert(a.a_, 1, f);
			}
			break;
	}
}

auto get_input()
{
	std::vector<action> va;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, a, b] = ctre::match<"swap position (\\d+) with position (\\d+)">(ln); m)
			va.emplace_back(act::swap_pos, a.to_number<uint8_t>(), b.to_number<uint8_t>());
		else
		if(auto[m, a, b] = ctre::match<"swap letter ([a-z]) with letter ([a-z])">(ln); m)
			va.emplace_back(act::swap_letter, a.view()[0], b.view()[0]);
		else
		if(auto[m, a] = ctre::match<"rotate left (\\d+) steps?">(ln); m)
			va.emplace_back(act::rotate_left, a.to_number<uint8_t>(), 0);
		else
		if(auto[m, a, b] = ctre::match<"move position (\\d+) to position (\\d+)">(ln); m)
			va.emplace_back(act::move, a.to_number<uint8_t>(), b.to_number<uint8_t>());
		else
		if(auto[m, a] = ctre::match<"rotate right (\\d+) steps?">(ln); m)
			va.emplace_back(act::rotate_right, a.to_number<uint8_t>(), 0);
		else
		if(auto[m, a] = ctre::match<"rotate based on position of letter ([a-z])">(ln); m)
			va.emplace_back(act::rotate_pos, a.view()[0], 0);
		else
		if(auto[m, a, b] = ctre::match<"reverse positions (\\d+) through (\\d+)">(ln); m)
			va.emplace_back(act::reverse, a.to_number<uint8_t>(), b.to_number<uint8_t>());
		else
			fmt::println("parse fail line {}", ln);
	}
	return va;
}

std::string pt1(auto const& in)
{
	timer t("p1");
	std::string pw = "abcdefgh";
	for(auto& x: in)
		do_act(x, pw);

	return pw;
}

std::string pt2(auto const& in)
{
	timer t("p2");
	std::string pw = "fbgdceah";
	for(auto i = in.rbegin(); i != in.rend(); ++i)
		do_ract(*i, pw);
	return pw;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

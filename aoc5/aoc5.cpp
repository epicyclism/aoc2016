#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <openssl/md5.h>
#include <fmt/format.h>

#include "timer.h"

#if defined (USE_OPENSSL)

auto next_md5(std::string_view salt, unsigned n)
{
	std::array<uint8_t, 16> md5;
	const unsigned key = 0x00f0ffff;
	auto nn = n;
	std::string wsp;
	while(1)
	{
		wsp = salt;
		wsp += std::to_string(nn);
		MD5(reinterpret_cast<unsigned char const*>(wsp.data()), wsp.size(), md5.data());
		if((key & *reinterpret_cast<unsigned const*>(md5.data())) == 0)
		{
			unsigned nxt_pass = *reinterpret_cast<unsigned const*>(md5.data()) >> 16;
			return std::make_pair(nn, nxt_pass) ;
		}
		++nn;
	}
}

char to_hex(unsigned x)
{
	if(x < 10)
		return x + '0';
	return x + 'a' - 10;
}

auto pt12(auto const& in)
{
	timer t("p2");
	std::string pw1="00000000";
	std::string pw2="ffffffff";
	unsigned nn = 0;
	std::array<bool, 8> ab;
	ab.fill(false);
	int p1 = 0;
	while(!std::ranges::all_of(ab, std::identity()))
	{
		auto[nnt, pc] = next_md5(in, nn);
		nn = nnt + 1;
		auto off = pc & 0x0f;
		if(p1< 8)
		{
			pw1[p1] = to_hex(off);
			++p1;
		}
		if( off < 8 && !ab[off])
		{
			pw2[off] = to_hex(pc >> 12);
			ab[off] = true;
		}
	}
	return std::make_pair(pw1, pw2);
}

int main(int ac, char**av)
{
	std::string in = "ugkcyxxp";
	if(ac > 1)
		in = av[1];
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
#else
int main()
{
	fmt::println("aoc5 : OpenSSL not found, install it and rebuild.");
}
#endif
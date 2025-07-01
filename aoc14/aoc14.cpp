#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <openssl/md5.h>
#include <fmt/format.h>

#include "timer.h"

#if defined (USE_OPENSSL)

using md5_t  = std::array<uint8_t, 16>;
using hash_t = std::array<char, 32>;

auto to_hex(md5_t const& m)
{
	hash_t h;
	auto hb = h.begin();
	for(auto x: m)
	{
		auto b = (x & 0xf0) >> 4;
		if(b < 10)
			*hb = b + '0';
		else
			*hb = b + 'a' - 10;
		++hb;
		b = x & 0x0f;
		if(b < 10)
			*hb = b + '0';
		else
			*hb = b + 'a' - 10;
		++hb;
	}
	return h;
}

auto next_md5(std::string_view salt, unsigned n)
{
	md5_t md5;
	std::string wsp(salt.begin(), salt.end());
	wsp += std::to_string(n);
	MD5(reinterpret_cast<unsigned char const*>(wsp.data()), wsp.size(), md5.data());
	return to_hex(md5);
}

char is_3(hash_t const& h)
{
	for(int n = 0; n < h.size() - 3; ++n)
		if(h[n] == h[n + 1] && h[n] == h[n + 2])
			return h[n];

	return ' ';
}

bool is_5(hash_t const& h, char k)
{
	return std::ranges::search_n(h, 5, k).size() == 5;
}

bool scan_5(std::vector<hash_t> const& vh, int index, char k)
{
	for(int t = index; t < index + 1000; ++t)
		if(is_5(vh[t], k))
			return true;
	return false;
}

int pt1(auto const& in)
{
	timer t("p1");
	std::vector<hash_t> vh;
	// preload
	for( int i = 0; i < 1001; ++i)
		vh.emplace_back(next_md5(in, i));
	int index = 0;
	int cnt = 0;
	while(1)
	{
		if(auto k = is_3(vh[index]); k != ' ')
		{
			cnt += scan_5(vh, index + 1, k);
			if(cnt == 63)
				break;
		}
		vh.emplace_back(next_md5(in, vh.size()));
		++index;
	}
	return index;
}
// 29847 too high
// 25327 too low

int pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

int main(int ac, char ** av)
{
	std::string in ("yjdafjpo");
	if( ac > 1)
		in = av[1];
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
#else
int main()
{
	fmt::println("aoc14 : OpenSSL not found, install it and rebuild.");
}
#endif
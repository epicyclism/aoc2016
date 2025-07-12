#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>

#include <openssl/md5.h>

#include <fmt/format.h>

#include "timer.h"

#if defined (USE_OPENSSL)

// DURL...
auto next_md5(std::string_view salt, std::string const& pth)
{
	std::array<bool, 4> rv;
	std::array<uint8_t, 16> md5;

	std::string wsp(salt);
	wsp += pth;
	MD5(reinterpret_cast<unsigned char const*>(wsp.data()), wsp.size(), md5.data());
	uint16_t k = *(uint16_t*)(md5.data());
	uint16_t m = 0x000f;
	uint16_t v = 0x000a;
	for(int i = 0; i < 4; ++i)
	{
		rv[i] = (k & m) > v;
		m <<= 4;
		v <<= 4;
	}
	return rv;
}

std::string pt1(auto const& in)
{
	timer t("p1");
	
	std::queue<std::pair<int, std::string>> q;
	q.emplace(0, "");
	while(!q.empty())
	{
		auto u = q.front();
		q.pop();
		if(u.first == 15)
			return u.second;
		auto nxt = next_md5(in, u.second);
		// DURL
		if(nxt[0] && u.first < 12)
			q.emplace(u.first + 4, u.second + 'D');
		if(nxt[1] && u.first > 3)
			q.emplace(u.first - 4, u.second + 'U');
		if(nxt[2] && u.first % 4 < 3)
			q.emplace(u.first + 1, u.second + 'R');
		if(nxt[3] && u.first % 4 != 0)
			q.emplace(u.first - 1, u.second + 'L');		
	}
	return "BOGUS";
}

int pt2(auto const& in)
{
	timer t("p2");
	int mx = 0;
	std::queue<std::pair<int, std::string>> q;
	q.emplace(0, "");
	while(!q.empty())
	{
		auto u = q.front();
		q.pop();
		if(u.first == 15)
			mx = u.second.size() > mx ? u.second.size() : mx;
		else
		{
			auto nxt = next_md5(in, u.second);
			// DURL
			if(nxt[0] && u.first < 12)
				q.emplace(u.first + 4, u.second + 'D');
			if(nxt[1] && u.first > 3)
				q.emplace(u.first - 4, u.second + 'U');
			if(nxt[2] && u.first % 4 < 3)
				q.emplace(u.first + 1, u.second + 'R');
			if(nxt[3] && u.first % 4 != 0)
				q.emplace(u.first - 1, u.second + 'L');
		}
	}
	return mx;
}

int main(int ac, char** av)
{
	std::string in = "pxxbnzuo";
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
	fmt::println("aoc5 : OpenSSL not found, install it and rebuild.");
}
#endif
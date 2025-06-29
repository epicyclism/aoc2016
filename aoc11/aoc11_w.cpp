#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

// four floors, so 2 bits per item
// five items * 2 and an elevator
// so 11x2 bits to describe state

constexpr std::string_view elts []{ "th", "pl", "pr", "ru", "st", ".   " };
constexpr std::string_view elts2a []{ "pl", "pr", "ru", "st", ".   " };
constexpr std::string_view elts2b []{ "th", "pl", "pr", "ru", "st", ".   " };
constexpr std::string_view elts2c []{ "el", "th", "pl", "pr", "ru", "st", ".   " };
constexpr std::string_view elts2 []{ "di", "el", "th", "pl", "pr", "ru", "st", ".   " };
constexpr std::string_view eltst[]{ "H", "L", ".  " };

constexpr std::string_view get_elt (int n) { return elts[n]; }
constexpr std::string_view get_elt2a(int n) { return elts2a[n]; }
constexpr std::string_view get_elt2b(int n) { return elts2b[n]; }
constexpr std::string_view get_elt2c(int n) { return elts2c[n]; }
constexpr std::string_view get_elt2(int n) { return elts2[n]; }
constexpr std::string_view get_eltt(int n) { return eltst[n]; }

template<int N, std::string_view (*F)(int)> struct state
{
	enum { n = N };

	uint8_t el_;
	uint8_t g_[N];
	uint8_t m_[N];

	uint8_t& gm(int e)
	{
		if (e < N)
			return g_[e];
		else
			return m_[e - N];
	}
	state() = delete;
	void expand(uint32_t u)
	{
		uint32_t msk = 0x00000003;
		uint32_t shft = 0;
		for (int e = 0; e < N; ++e)
		{
			m_[e] = (u & msk) >> shft;
			msk <<= 2;
			shft += 2;
			g_[e] = (u & msk) >> shft;
			msk <<= 2;
			shft += 2;
		}
		el_ = (u & msk) >> shft;
	}
	uint32_t condense() const
	{
		uint32_t u{ 0 };
		uint32_t shft = 0;
		for (int e = 0; e < N; ++e)
		{
			u |= (uint32_t(m_[e]) << shft);
			shft += 2;
			u |= (uint32_t(g_[e]) << shft);
			shft += 2;
		}
		u |= (uint32_t(el_)) << shft;
		return u;
	}
	std::vector<uint32_t> next_moves()
	{
		std::vector<uint32_t> rv;

		auto el = el_;
		if (el < 3) // can go up!
		{
			++el_;
			// move one
			for (int e = 0; e < 2 * N; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					++re;
					if (is_valid(*this))
						rv.emplace_back(condense());
					--re;
				}
			}
			// move two
			for (int e = 0; e < 2 * N - 1; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					++re;
					for (int e2 = e + 1; e2 < 2 * N; ++e2)
					{
						auto& re2 = gm(e2);
						if (re2 == el)
						{
							++re2;
							if (is_valid(*this))
								rv.emplace_back(condense());
							--re2;
						}
					}
					--re;
				}
			}
			--el_;
		}
		if (el > 0) // can go down!
		{
			--el_;
			// move one
			for (int e = 0; e < 2 * N; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					--re;
					if (is_valid(*this))
						rv.emplace_back(condense());
					++re;
				}
			}
			// move two
			for (int e = 0; e < 2 * N - 1; ++e)
			{
				auto& re = gm(e);
				if (re == el)
				{
					--re;
					for (int e2 = e + 1; e2 < 2 * N; ++e2)
					{
						auto& re2 = gm(e2);
						if (re2 == el)
						{
							--re2;
							if (is_valid(*this))
								rv.emplace_back(condense());
							++re2;
						}
					}
					++re;
				}
			}
			++el_;
		}
		return rv;
	}
	void print()
	{
		for (int f = 3; f >= 0; --f)
		{
			std::cout << "F" << f + 1 << (el_ == f ? " E  " : " .  ");
			for (int e = 0; e < N; ++e)
			{
				if (g_[e] == f)
					std::cout << F(e) << "G ";
				else
					std::cout << F(N);
				if (m_[e] == f)
					std::cout << F(e) << "M ";
				else
					std::cout << F(N);
			}
			std::cout << "\n";
		}
	}
	uint32_t max_state() const
	{
		auto r = 1 << (2 * N + 1) * 2;
		return r;
	}
};

using test_st_t = state<2, get_eltt>;
using      st_t = state<5, get_elt>;
using      st2a_t = state<4, get_elt2a>;
using      st2b_t = state<5, get_elt2b>;
using      st2c_t = state<6, get_elt2c>;
using      st2_t = state<7, get_elt2>;

// is valid if no unaccompanied M and G, either is OK
//
template<typename T> bool is_valid(T const& st)
{
	// for each floor, state is invalid if there is any generator and an unmatched microchip
	for (int f = 0; f < 4; ++f)
	{
		bool unmatched_m = false;
		bool any_g = false;
		for (int e = 0; e < T::n; ++e)
		{
			if (st.m_[e] == f && !(st.g_[e] == f))
				unmatched_m = true;
			if (st.g_[e] == f)
				any_g = true;
		}
		if (unmatched_m && any_g)
			return false;
	}
	return true;
}

void test()
{
	test_st_t st{ 0x00, 0x01, 0x02, 0x00, 0x00 };
	st.print();
	std::cout << (is_valid(st) ? "is valid!\n" : "is invalid....\n");
	test_st_t sta{ 0x00, 0x01, 0x02, 0x00, 0x01 };
	sta.print();
	std::cout << (is_valid(sta) ? "is valid!\n" : "is invalid....\n");
	//           E   thG   plG   prG   ruG   stG   thM   plM   prM   ruM   stM
	st_t st2{ 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st2.print();
	std::cout << (is_valid(st2) ? "is valid!\n" : "is invalid....\n");
	st_t st2a{ 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02 };
	st2a.print();
	std::cout << (is_valid(st2a) ? "is valid!\n" : "is invalid....\n");
	auto con = st.condense();
	std::cout << "st condenses to " << con << "\n";
	test_st_t stb{ 0x00, 0x00, 0x00, 0x00, 0x00 };
	stb.expand(con);
	std::cout << "And expands to\n";
	stb.print();
	con = st2.condense();
	std::cout << "st2 condenses to " << con << "\n";
	st_t st2b{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	st2b.expand(con);
	std::cout << "And expands to\n";
	st2b.print();
	std::cout << "Testing moves...\n";
	st.expand(0x189);
	st.print();
	auto vm = st.next_moves();
	std::cout << "Got " << vm.size() << " moves.\n";
	for (auto u : vm)
	{
		test_st_t stt{ 0 };
		stt.expand(u);
		stt.print();
		std::cout << "\n";
	}
}

using wt = boost::property<boost::edge_weight_t, int>;
using grph = boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS, boost::no_property, wt > ;
int cnt = 0;

template<typename T> void populate(uint32_t u, grph& g)
{
	T st{ 0 };
	st.expand(u);
	auto vm = st.next_moves();
	if (vm.empty())
		return;
	for (auto ue : vm)
	{
		boost::add_edge(u, ue, 1, g);
		++cnt;
	}
	for (auto ue : vm)
	{
		if (boost::out_degree(ue, g) == 0)
		{
			populate<T>(ue, g);
		}
	}
}

template<typename T> void populate2(grph& g)
{
	T st{ 0 };

	for (uint32_t u = 0; u < st.max_state(); ++u)
	{
		st.expand(u);
		auto vm = st.next_moves();
		for (auto ue : vm)
		{
			boost::add_edge(u, ue, 1, g);
			++cnt;
		}
	}
}

void print_graph(grph const& g)
{
	auto v = boost::vertices(g);
	std::cout << std::hex;
	std::for_each(v.first, v.second, [&g](auto i)
		{
			if (boost::out_degree(i, g))
			{
				std::cout << "x" << i << " : ";
				auto al = boost::adjacent_vertices(i, g);
				std::for_each(al.first, al.second, [](auto a)
					{
						std::cout << "x" << a << ", ";
					});
				std::cout << "\n";
			}
		});
}

void test_pt1()
{
	grph g;

	test_st_t st { 0x00, 0x01, 0x02, 0x00, 0x00 };
	test_st_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u = st.condense();
	auto uf = stf.condense();

//	populate<test_st_t>(u, g);
	populate2<test_st_t>(g);
//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}

void pt1()
{
	grph g;

	//           E   thG   plG   prG   ruG   stG   thM   plM   prM   ruM   stM
	st_t st { 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u  = st.condense();
	auto uf = stf.condense();

	populate2<st_t>(g);
//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}

void pt2a()
{
	grph g;

	//           E   plG   prG   ruG   stG   plM   prM   ruM   stM
	st2a_t st { 0x00, 0x00, 0x02, 0x02, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st2a_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u = st.condense();
	auto uf = stf.condense();

	st.print();

	populate2<st2a_t>(g);
	//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}
void pt2b()
{
	grph g;

	//           E   thG   plG   prG   ruG   stG   thM   plM   prM   ruM   stM
	st2b_t st{ 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st2b_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u = st.condense();
	auto uf = stf.condense();

	st.print();

	populate2<st2b_t>(g);
	//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}

void pt2c()
{
	grph g;

	//           E   elG   thG   plG   prG   ruG   stG   elM   thM   plM   prM   ruM   stM
	st2c_t st{ 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st2c_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u = st.condense();
	auto uf = stf.condense();

	st.print();

	populate2<st2c_t>(g);
	//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}

void pt2()
{
	grph g;

	//           E   diG   elG   thG   plG   prG   ruG   stG   diM   elM   thM   plM   prM   ruM   stM
	st2_t st { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01 };
	st2_t stf{ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 };
	auto u  = st.condense();
	auto uf = stf.condense();

	st.print();

	populate2<st2_t>(g);
//	print_graph(g);

	std::cout << "Got " << boost::num_vertices(g) << ", " << cnt << " states!\n";

	// apply the boost magic
	std::vector<int> dist(boost::num_vertices(g));
	// start with the torch
	boost::dijkstra_shortest_paths(g, boost::vertex(u, g), boost::distance_map(dist.data()));

	std::cout << "dist at " << uf << " = " << dist[uf] << "\n";
}

int main()
{
	test();
	test_pt1();
	pt1();
	pt2a();
	pt2b();
	pt2c();
	// pt2 is then output of pt2c + constant diff. Ugly.
}

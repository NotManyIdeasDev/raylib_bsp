#pragma once
#include <random>

class RNG
{
public:
	//Proudly stolen from stackoverflow
	template<typename Numeric, typename Generator = std::mt19937>
	static Numeric GenerateNumber(Numeric from, Numeric to)
	{
		thread_local static Generator gen(std::random_device{}());
		using dist_type = typename std::conditional
		<
			std::is_integral<Numeric>::value,
			std::uniform_int_distribution<Numeric>,
			std::uniform_real_distribution<Numeric>
		>::type;

		thread_local static dist_type dist;
		return dist(gen, typename dist_type::param_type{ from, to });
	}
};
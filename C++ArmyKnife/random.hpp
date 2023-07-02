
#pragma once
#include "concepts.hpp"
namespace
{
	template<std::input_or_output_iterator It>
	auto shuffle_and_sample_repeatedly(It beg, It end, size_t n) -> std::vector<std::iter_value_t<It>>
	{
		std::random_device rd{};
		std::mt19937_64 rng{ rd() };
		std::vector<std::iter_value_t<It>> samples(n);
		auto it{ samples.begin() };
		while (it != samples.end())
		{
			std::shuffle(beg, end, rng);
			std::sample(beg, end, it, 1, rng);
			it++;
		}
		return samples;
	}

	template<std::input_or_output_iterator It>
	auto histogram(It beg, It end) -> std::unordered_map<std::iter_value_t<It>, size_t>
	{
		std::unordered_map<std::iter_value_t<It>, size_t> hist{};
		auto it{ beg };
		while (it != end)
		{
			hist[*it]++;
			it = std::next(it);
		}
		return hist;
	}
}
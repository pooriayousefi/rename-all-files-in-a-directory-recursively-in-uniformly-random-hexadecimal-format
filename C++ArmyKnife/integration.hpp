
#pragma once
#include "concepts.hpp"
#include "literals.hpp"
namespace
{
	namespace integration
	{
		// trapzoidal
		template<std::floating_point T, real_valued_function<T> F>
		auto trapzoidal(T x0, T xn, size_t n, F f) -> T
		{
			auto h{ (xn - x0) / n };
			std::vector<T> fx(n + 1, T{ 0 });
			for (auto i{ 0_sz }; i < n; ++i)
				fx[i] = f(x0 + (i * h));
			fx[n] = f(xn);

			return (h / T{ 2 })*
				(fx[0] + T{ 2 } *std::reduce(std::execution::seq,
					std::next(fx.begin()),
					std::prev(std::prev(fx.end())),
					T{ 0 },
					std::plus<T>{}) + fx[n]);
		}

		// simpson 1/3
		template<std::floating_point T, real_valued_function<T> F>
		auto simpson13(T x0, T xn, size_t n, F f) -> T
		{
			auto h{ (xn - x0) / n };
			std::vector<T> fx(n + 1, T{ 0 });
			for (auto i{ 0_sz }; i < n; ++i)
				fx[i] = f(x0 + (i * h));
			fx[n] = f(xn);

			auto key{ true };
			auto s1{ T{0} }, s2{ T{0} };
			std::for_each(std::execution::seq,
				std::next(fx.begin()),
				std::prev(std::prev(fx.end())),
				[&key, &s1, &s2](auto& xi) { (key ? s1 += xi : s2 += xi); key = !key; });
			return (h / T{ 3 })* (fx[0] + T{ 4 } *s1 + T{ 2 } *s2 + fx[n]);
		}
	}
}

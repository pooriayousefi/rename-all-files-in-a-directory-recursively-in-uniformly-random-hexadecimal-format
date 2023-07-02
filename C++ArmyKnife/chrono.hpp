
#pragma once
#include "concepts.hpp"
namespace
{
	// high-resolution chronometer
	namespace high_resolution
	{
		// time-point type alias
		using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

		// high resolution clock function
		auto clock = []() -> timepoint { return std::chrono::high_resolution_clock::now(); };

		// elapsed time (in seconds) calculator function
		auto elapsed_time = [](timepoint ti, timepoint tf) -> double { return std::chrono::duration<double>(tf - ti).count(); };
	}

	// countdown function
	template<procedure<int> F>
	auto countdown(int t, F f)
	{
		if (t < 0)
			throw std::invalid_argument("in contdown function, time argument must be greater than equal to zero.");
		while (t >= 0)
		{
			std::invoke(f, t--);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}
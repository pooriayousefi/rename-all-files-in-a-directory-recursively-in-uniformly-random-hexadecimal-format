
#pragma once
#include "stdh.hpp"
namespace
{
	// modular application template
	template<size_t NModules>
	class application
	{
	public:
		using timepoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
		application()
			:modules_{}, 
			runtimes_{ 0.0 } {}
		virtual ~application() = default;
		auto set_modules(std::array<std::function<void()>, NModules>& modules) -> void
		{
			std::swap(modules_, modules);
		}
		auto run()->void
		{
			std::transform(std::execution::seq,
				modules_.begin(),
				modules_.end(),
				runtimes_.begin(),
				[this](auto& m)
				{
					auto ti{ high_resolution_clock() };
					m();
					auto tf{ high_resolution_clock() };
					return high_resolution_chronometer(ti, tf);
				});
		}
		auto operator()() -> void 
		{ 
			run();
		}
		auto modules_runtimes() const -> std::array<double, NModules> 
		{
			return runtimes_;
		}
		auto runtime() const -> double
		{
			return std::reduce(std::execution::seq,
				runtimes_.cbegin(),
				runtimes_.cend(),
				0.0,
				std::plus<double>{});
		}
	private:
		std::array<std::function<void()>, NModules> modules_;
		std::array<double, NModules> runtimes_;
		auto high_resolution_clock() const -> timepoint
		{ 
			return std::chrono::high_resolution_clock::now();
		}
		auto high_resolution_chronometer(timepoint ti, timepoint tf) const -> double 
		{ 
			return std::chrono::duration<double>(tf - ti).count(); 
		}
	};
}

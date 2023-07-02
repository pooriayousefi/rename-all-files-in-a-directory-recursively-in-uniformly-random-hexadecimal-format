
#pragma once
#include "type_aliases.hpp"
namespace
{
	// meta space
	namespace meta
	{
		// meta data
		template<typename X>
		class data
		{
		public:
			X value;
			data() 
				:value{} {}
			data(X inival) 
				:value{ inival } {}
			data(const data<X>&) = default;
			data(data<X>&&) noexcept = default;
			virtual ~data() = default;
			auto operator=(const data<X>&)->data & = default;
			auto operator=(data<X>&&) noexcept->data & = default;
			template<std::invocable<X> F> decltype(auto) apply(F f) 
			{ 
				return data<std::invoke_result_t<F, X> >(f(value));
			}
			template<typename H, typename... Fs> decltype(auto) pipe(H h, Fs... fs)
			{
				auto original_value{ value };
				auto result_value{ pipe_(h, fs...) };
				value = original_value;
				return result_value;
			}
			template<typename H, typename... Fs> decltype(auto) compose(H h, Fs... fs)
			{
				auto original_value{ value };
				auto result_value{ compose_(h, fs...) };
				value = original_value;
				return result_value;
			}
		private:
			template<typename H, typename... Fs> decltype(auto) pipe_(H h, Fs... fs)
			{
				value = h(value);
				return pipe_(fs...);
			}
			template<typename F, typename G> decltype(auto) pipe_(F f, G g)
			{
				value = f(value);
				return g(value);
			}
			template<typename H, typename... Fs> decltype(auto) compose_(H h, Fs... fs)
			{
				value = compose_(fs...);
				return h(value);
			}
			template<typename F, typename G> decltype(auto) compose_(F f, G g)
			{
				value = g(value);
				return f(value);
			}
		};

		// meta-loop
		template<size_t N, typename I = std::make_index_sequence<N>>
		struct loop;

		template<size_t N, size_t... Is>
		struct loop<N, std::index_sequence<Is...>> 
		{
			loop(std::function<void(const size_t)> f)
			{
				(std::invoke(f, Is), ...);
			}
		};

		template<size_t W, size_t H>
		struct loop2
		{
			template<typename F>
			auto operator()(F f) -> void
			{
				loop<H> lh(
					[f](const size_t h)
					{
						loop<W> lw(
							[f, h](const size_t w)
							{
								std::invoke(f, w, h);
							}
						);
					}
				);
			}
			template<typename T>
			auto operator()(std::basic_ostream<T, std::char_traits<T>>& os) -> void
			{
				loop<H> lh(
					[&os](const size_t h)
					{
						loop<W> lw(
							[&os, h](const size_t w)
							{
								os << w << h << ' ';
							}
						);
						os << std::endl;
					}
				);
				os << std::endl;
			}
			template<typename T>
			auto operator()(std::basic_ofstream<T, std::char_traits<T>>& os) -> void
			{
				loop<H> lh(
					[&os](const size_t h)
					{
						loop<W> lw(
							[&os, h](const size_t w)
							{
								os << w << h << ' ';
							}
						);
						os << std::endl;
					}
				);
				os << std::endl;
			}
		};

		template<size_t W, size_t H, size_t D>
		struct loop3
		{
			template<typename F>
			auto operator()(F f) -> void
			{
				loop<D> ld(
					[f](const size_t d)
					{
						loop<H> lh(
							[f, d](const size_t h)
							{
								loop<W> lw(
									[f, d, h](const size_t w)
									{
										std::invoke(f, w, h, d);
									}
								);
							}
						);
					}
				);
			}
			template<typename T>
			auto operator()(std::basic_ostream<T, std::char_traits<T>>& os) -> void
			{
				loop<D> ld(
					[&os](const size_t d)
					{
						loop<H> lh(
							[&os, d](const size_t h)
							{
								loop<W> lw(
									[&os, d, h](const size_t w)
									{
										os << w << h << d << ' ';
									}
								);
								os << std::endl;
							}
						);
						os << std::endl;
					}
				);
				os << std::endl;
			}
			template<typename T>
			auto operator()(std::basic_ofstream<T, std::char_traits<T>>& os) -> void
			{
				loop<D> ld(
					[&os](const size_t d)
					{
						loop<H> lh(
							[&os, d](const size_t h)
							{
								loop<W> lw(
									[&os, d, h](const size_t w)
									{
										os << w << h << d << ' ';
									}
								);
								os << std::endl;
							}
						);
						os << std::endl;
					}
				);
				os << std::endl;
			}
		};
	}

	template<encoding T, typename X>
	inline auto operator<<(output_stream<T>& os, const meta::data<X>& x) -> output_stream<T>&
	{
		os << x.value;
		return os;
	}
	template<encoding T, typename X>
	inline auto operator<<(output_file_stream<T>& os, const meta::data<X>& x) -> output_file_stream<T>&
	{
		os << x.value;
		return os;
	}
}
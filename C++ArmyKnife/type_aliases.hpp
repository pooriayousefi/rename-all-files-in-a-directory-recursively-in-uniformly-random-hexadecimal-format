
#pragma once
#include "concepts.hpp"
namespace
{
	template<encoding T>
	using string = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

	template<encoding T>
	using string_view = std::basic_string_view<T, std::char_traits<T>>;

	template<encoding T>
	using input_stream = std::basic_istream<T, std::char_traits<T>>;

	template<encoding T>
	using output_stream = std::basic_ostream<T, std::char_traits<T>>;

	template<encoding T>
	using stream = std::basic_iostream<T, std::char_traits<T>>;

	template<encoding T>
	using input_file_stream = std::basic_ifstream<T, std::char_traits<T>>;

	template<encoding T>
	using output_file_stream = std::basic_ofstream<T, std::char_traits<T>>;

	template<encoding T>
	using file_stream = std::basic_fstream<T, std::char_traits<T>>;
}
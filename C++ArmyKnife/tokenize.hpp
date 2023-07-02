
#pragma once
#include "type_aliases.hpp"
namespace
{
	// tokenize to vector template
	template<encoding T>
	struct tokenize_to_vector
	{
		inline auto operator()(const string<T>& str, const string<T>& delimiters) -> std::vector<string<T>>
		{
			std::vector<string<T>> tokens{};

			// skip delimiters at begining
			auto last_pos = str.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = str.find_first_of(delimiters, last_pos);

			while (pos != string<T>::npos || last_pos != string<T>::npos)
			{
				// find a token, add it to the vector
				tokens.emplace_back(str.substr(last_pos, pos - last_pos));

				// skip delimiters
				last_pos = str.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = str.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}

		inline auto operator()(string_view<T> strview, string_view<T> delimiters) -> std::vector<string_view<T>>
		{
			std::vector<string_view<T>> tokens{};

			// skip delimiters at begining
			auto last_pos = strview.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = strview.find_first_of(delimiters, last_pos);

			while (pos != string_view<T>::npos || last_pos != string_view<T>::npos)
			{
				// find a token, add it to the vector
				tokens.emplace_back(strview.substr(last_pos, pos - last_pos));

				// skip delimiters
				last_pos = strview.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = strview.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}
	};

	// tokenize to vector instantiation
	using tokenize_narrow_encoded_string_to_vector = tokenize_to_vector<char>;
	using tokenize_wide_encoded_string_to_vector = tokenize_to_vector<wchar_t>;
	using tokenize_utf8_string_to_vector = tokenize_to_vector<char8_t>;
	using tokenize_utf16_string_to_vector = tokenize_to_vector<char16_t>;
	using tokenize_utf32_string_to_vector = tokenize_to_vector<char32_t>;
	using tokenize_binary_string_to_vector = tokenize_to_vector<std::byte>;

	// tokenize to hash set template
	template<encoding T>
	struct tokenize_to_hashset
	{
		inline auto operator()(const string<T>& str, const string<T>& delimiters) -> std::unordered_set<string<T>>
		{
			std::unordered_set<string<T>> tokens{};

			// skip delimiters at begining
			auto last_pos = str.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = str.find_first_of(delimiters, last_pos);

			while (pos != string<T>::npos || last_pos != string<T>::npos)
			{
				// find a token, add it to the vector
				tokens.emplace(str.substr(last_pos, pos - last_pos));

				// skip delimiters
				last_pos = str.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = str.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}

		inline auto operator()(string_view<T> strview, string_view<T> delimiters) -> std::unordered_set<string_view<T>>
		{
			std::unordered_set<string_view<T>> tokens{};

			// skip delimiters at begining
			auto last_pos = strview.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = strview.find_first_of(delimiters, last_pos);

			while (pos != string_view<T>::npos || last_pos != string_view<T>::npos)
			{
				// find a token, add it to the vector
				tokens.emplace(strview.substr(last_pos, pos - last_pos));

				// skip delimiters
				last_pos = strview.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = strview.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}
	};

	// tokenize to hash set instantiation
	using tokenize_narrow_encoded_string_to_hashset = tokenize_to_hashset<char>;
	using tokenize_wide_encoded_string_to_hashset = tokenize_to_hashset<wchar_t>;
	using tokenize_utf8_string_to_hashset = tokenize_to_hashset<char8_t>;
	using tokenize_utf16_string_to_hashset = tokenize_to_hashset<char16_t>;
	using tokenize_utf32_string_to_hashset = tokenize_to_hashset<char32_t>;
	using tokenize_binary_string_to_hashset = tokenize_to_hashset<std::byte>;
	
	// tokenize to hash map template
	template<encoding T>
	struct tokenize_to_hashmap
	{
		inline auto operator()(const string<T>& str, const string<T>& delimiters) -> std::unordered_map<string<T>, size_t>
		{
			std::unordered_map<string<T>, size_t> tokens{};

			// skip delimiters at begining
			auto last_pos = str.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = str.find_first_of(delimiters, last_pos);

			while (pos != string<T>::npos || last_pos != string<T>::npos)
			{
				// find a token, add it to the vector
				tokens[str.substr(last_pos, pos - last_pos)]++;

				// skip delimiters
				last_pos = str.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = str.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}

		inline auto operator()(string_view<T> strview, string_view<T> delimiters) -> std::unordered_map<string_view<T>, size_t>
		{
			std::unordered_map<string_view<T>, size_t> tokens{};

			// skip delimiters at begining
			auto last_pos = strview.find_first_not_of(delimiters, 0);

			// find first non-delimiter
			auto pos = strview.find_first_of(delimiters, last_pos);

			while (pos != string_view<T>::npos || last_pos != string_view<T>::npos)
			{
				// find a token, add it to the vector
				tokens[strview.substr(last_pos, pos - last_pos)]++;

				// skip delimiters
				last_pos = strview.find_first_not_of(delimiters, pos);

				// find next non-delimiter
				pos = strview.find_first_of(delimiters, last_pos);
			}

			return tokens;
		}
	};

	// tokenize to hash map instantiation
	using tokenize_narrow_encoded_string_to_hashmap = tokenize_to_hashmap<char>;
	using tokenize_wide_encoded_string_to_hashmap = tokenize_to_hashmap<wchar_t>;
	using tokenize_utf8_string_to_hashmap = tokenize_to_hashmap<char8_t>;
	using tokenize_utf16_string_to_hashmap = tokenize_to_hashmap<char16_t>;
	using tokenize_utf32_string_to_hashmap = tokenize_to_hashmap<char32_t>;
	using tokenize_binary_string_to_hashmap = tokenize_to_hashmap<std::byte>;
}
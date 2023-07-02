
#pragma once
#include "literals.hpp"
#include "filesystem.hpp"
#include "tokenize.hpp"
namespace
{
	template<encoding T>
	struct to_lowercase
	{
		inline auto operator()(const string<T>& str) -> string<T>
		{
			string<T> lcstr{};
			lcstr.reserve(str.size());
			std::transform(std::execution::seq,
				str.cbegin(),
				str.cend(),
				lcstr.begin(),
				[](const auto& elem) { return std::tolower(elem); });
			return lcstr;
		}
		inline auto operator()(string_view<T> strview) -> string<T>
		{
			string<T> lcstr{};
			lcstr.reserve(strview.size());
			std::transform(std::execution::seq,
				strview.cbegin(),
				strview.cend(),
				lcstr.begin(),
				[](const auto& elem) { return std::tolower(elem); });
			return lcstr;
		}
	};

	template<encoding T>
	struct to_uppercase
	{
		inline auto operator()(const string<T>& str) -> string<T>
		{
			string<T> lcstr{};
			lcstr.reserve(str.size());
			std::transform(std::execution::seq,
				str.cbegin(),
				str.cend(),
				lcstr.begin(),
				[](const auto& elem) { return std::toupper(elem); });
			return lcstr;
		}
		inline auto operator()(string_view<T> strview) -> string<T>
		{
			string<T> lcstr{};
			lcstr.reserve(strview.size());
			std::transform(std::execution::seq,
				strview.cbegin(),
				strview.cend(),
				lcstr.begin(),
				[](const auto& elem) { return std::toupper(elem); });
			return lcstr;
		}
	};

	using native_narrow_encoded_string_to_lowercase = to_lowercase<char>;
	using native_wide_encoded_string_to_lowercase = to_lowercase<wchar_t>;
	using utf8_string_to_lowercase = to_lowercase<char8_t>;
	using utf16_string_to_lowercase = to_lowercase<char16_t>;
	using utf32_string_to_lowercase = to_lowercase<char32_t>;

	using native_narrow_encoded_string_to_uppercase = to_uppercase<char>;
	using native_wide_encoded_string_to_uppercase = to_uppercase<wchar_t>;
	using utf8_string_to_uppercase = to_uppercase<char8_t>;
	using utf16_string_to_uppercase = to_uppercase<char16_t>;
	using utf32_string_to_uppercase = to_uppercase<char32_t>;

	template<encoding T>
	struct specialize
	{
		auto operator()(std::filesystem::path template_file_path_extension,
			string_view<T> pattern_view,
			std::span<string_view<T>> tokenized_filler_line_i,
			string<T> template_file_contents_copy) -> void
		{
			auto fit = tokenized_filler_line_i.begin();

			std::filesystem::path new_file_i_path(string<T>{ *fit });
			new_file_i_path += template_file_path_extension;

			for (auto& replacing_keyword : tokenized_filler_line_i)
				template_file_contents_copy.replace(template_file_contents_copy.find(pattern_view), pattern_view.size(), replacing_keyword);

			write_file_in_overwriting_fashion_if_existed<T> writer{};
			writer(new_file_i_path, template_file_contents_copy);
		}
	};

	auto specialize_template_text = []<encoding T>(std::filesystem::path template_file_path,
		std::filesystem::path fillers_file_path,
		string_view<T> pattern_view,
		string_view<T> fillers_delimiter_view)
	{
		auto template_file_path_extension{ template_file_path.extension() };
		read_file<T> reader{};
		auto template_file_contents{ reader(template_file_path) };
		read_file_line_by_line<T> line_by_line_reader{};
		auto fillers_file_contents{ line_by_line_reader(fillers_file_path) };
		auto n{ fillers_file_contents.size() };

		std::vector<std::vector<string_view<T>>> tokenized_fillers_contents{};
		tokenized_fillers_contents.reserve(n);

		tokenize_to_vector<T> tokenizer{};
		for (auto& l : fillers_file_contents)
			tokenized_fillers_contents.emplace_back(tokenizer(string_view<T>{ l }, fillers_delimiter_view));

		std::vector<std::future<void>> launcher{};
		launcher.reserve(n);

		for (auto i{ 0_sz }; i < n; ++i)
			launcher.emplace_back(std::async(std::launch::async,
				specialize<T>{},
				template_file_path_extension,
				pattern_view,
				std::span(tokenized_fillers_contents[i]),
				template_file_contents));
		for (auto& l : launcher)
			l.get();
	};
}
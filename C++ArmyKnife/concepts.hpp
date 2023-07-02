
#pragma once
#include "stdh.hpp"
namespace
{
	template<typename T>
	concept encoding = std::is_same_v<T, char> ||
		std::is_same_v<T, wchar_t> ||
		std::is_same_v<T, char8_t> ||
		std::is_same_v<T, char16_t> ||
		std::is_same_v<T, char32_t> ||
		std::is_same_v<T, std::byte>;

	template<typename T>
	concept string_family =
		std::is_same_v<T, std::string> ||
		std::is_same_v<T, std::wstring> ||
		std::is_same_v<T, std::u8string> ||
		std::is_same_v<T, std::u16string> ||
		std::is_same_v<T, std::u32string> ||
		std::is_same_v<T, std::basic_string<std::byte, std::char_traits<std::byte>, std::allocator<std::byte>>>;

	template<typename T>
	concept string_view_family =
		std::is_same_v<T, std::string_view> ||
		std::is_same_v<T, std::wstring_view> ||
		std::is_same_v<T, std::u8string_view> ||
		std::is_same_v<T, std::u16string_view> ||
		std::is_same_v<T, std::u32string_view> ||
		std::is_same_v<T, std::basic_string_view<std::byte, std::char_traits<std::byte>>>;

	template<typename F, typename... Args>
	concept procedure = std::is_invocable_r_v<void, F, Args...>;

	template<typename F, typename... Args>
	concept function =
		std::invocable<F, Args...> &&
		!std::is_same_v<std::invoke_result_t<F, Args...>, void>;

	template<typename T>
	concept default_constructible = std::is_default_constructible_v<T>;

	template<typename T, typename... Ts>
	concept constrained_on = (std::is_same_v<T, Ts> || ...);

	template<typename F, typename... Xs>
	concept real_valued_function = std::invocable<F, Xs...> &&
		std::floating_point<Xs...> &&
		std::floating_point<std::invoke_result_t<F, Xs...>>;
}

#pragma once
namespace
{
	// size literals
	namespace size_literals_internals
	{
		template<char CHARDIGIT>
		struct character_to_size_t_convert
		{
			static constexpr size_t value = static_cast<size_t>(CHARDIGIT) - static_cast<size_t>('0');
		};

		template<size_t EXPO>
		struct exponent
		{
			static constexpr size_t value = static_cast<size_t>(10) * exponent<EXPO - static_cast<size_t>(1)>::value;
		};

		template<>
		struct exponent<static_cast<size_t>(1)>
		{
			static constexpr size_t value = static_cast<size_t>(1);
		};

		template<size_t DIGIT, size_t DIGITPOS>
		struct digit
		{
			static constexpr size_t value = DIGIT * exponent<DIGITPOS>::value;
		};

		template<char FIRST, char... REST>
		struct size_literal
		{
			static_assert(FIRST == '0' || FIRST == '1' || FIRST == '2' || FIRST == '3' || FIRST == '4' ||
				FIRST == '5' || FIRST == '6' || FIRST == '7' || FIRST == '8' || FIRST == '9', "invalid digit");

			static constexpr size_t value =
				digit<character_to_size_t_convert<FIRST>::value, sizeof...(REST) + static_cast<size_t>(1)>::value +
				size_literal<REST...>::value;
		};

		template<> struct size_literal<'0'> { static constexpr size_t value = character_to_size_t_convert<'0'>::value; };
		template<> struct size_literal<'1'> { static constexpr size_t value = character_to_size_t_convert<'1'>::value; };
		template<> struct size_literal<'2'> { static constexpr size_t value = character_to_size_t_convert<'2'>::value; };
		template<> struct size_literal<'3'> { static constexpr size_t value = character_to_size_t_convert<'3'>::value; };
		template<> struct size_literal<'4'> { static constexpr size_t value = character_to_size_t_convert<'4'>::value; };
		template<> struct size_literal<'5'> { static constexpr size_t value = character_to_size_t_convert<'5'>::value; };
		template<> struct size_literal<'6'> { static constexpr size_t value = character_to_size_t_convert<'6'>::value; };
		template<> struct size_literal<'7'> { static constexpr size_t value = character_to_size_t_convert<'7'>::value; };
		template<> struct size_literal<'8'> { static constexpr size_t value = character_to_size_t_convert<'8'>::value; };
		template<> struct size_literal<'9'> { static constexpr size_t value = character_to_size_t_convert<'9'>::value; };
	}

	template<char... LITERAL>
	inline constexpr size_t operator"" _sz() { return size_literals_internals::size_literal<LITERAL...>::value; }

	template<char... LITERAL>
	inline constexpr size_t operator"" _SZ() { return size_literals_internals::size_literal<LITERAL...>::value; }

	// binary literals
	using byte8 = unsigned char;
	using byte16 = unsigned short;
	using byte32 = unsigned int;

	namespace binary_literals_internals
	{
		template<typename CharT, char... bits>
		struct binary_struct;

		template<typename CharT, char... bits>
		struct binary_struct<CharT, '0', bits...>
		{
			static constexpr CharT value{ binary_struct<CharT, bits...>::value };
		};

		template<typename CharT, char... bits>
		struct binary_struct<CharT, '1', bits...>
		{
			static constexpr CharT value{ static_cast<CharT>(1 << sizeof...(bits)) |
				binary_struct<CharT, bits...>::value };
		};

		template<typename CharT>
		struct binary_struct<CharT>
		{
			static constexpr CharT value{ 0 };
		};
	}

	template<char... bits>
	constexpr byte8 operator"" _b8()
	{
		static_assert(sizeof...(bits) <= 8, "binary literal b8 must be up to 8 digits long");
		return binary_literals_internals::binary_struct<byte8, bits...>::value;
	}

	template<char... bits>
	constexpr byte16 operator"" _b16()
	{
		static_assert(sizeof...(bits) <= 16, "binary literal b16 must be up to 16 digits long");
		return binary_literals_internals::binary_struct<byte16, bits...>::value;
	}

	template<char... bits>
	constexpr byte32 operator"" _b32()
	{
		static_assert(sizeof...(bits) <= 32, "binary literal b32 must be up to 32 digits long");
		return binary_literals_internals::binary_struct<byte32, bits...>::value;
	}
}
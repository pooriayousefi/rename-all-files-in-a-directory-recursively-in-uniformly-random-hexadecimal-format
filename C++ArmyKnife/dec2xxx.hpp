
#pragma once
#include "type_aliases.hpp"
namespace
{
	// decimal to binary converter
	template<std::integral I>
	auto convert_decimal_to_binary(I n, std::string& s) -> void
	{
		s.clear();
		if (n == (I)0)
			s += std::to_string((I)0);
		else
		{
			while (n != (I)0)
			{
				s += std::to_string(n % (I)2);
				n /= (I)2;
			}
			std::reverse(s.begin(), s.end());
		}
	}

	template<std::integral I>
	auto convert_decimal_to_binary(I n, std::wstring& s) -> void
	{
		s.clear();
		if (n == (I)0)
			s += std::to_wstring((I)0);
		else
		{
			while (n != (I)0)
			{
				s += std::to_wstring(n % (I)2);
				n /= (I)2;
			}
			std::reverse(s.begin(), s.end());
		}
	}

	// decimal to hexadecimal converter
	template<std::integral I>
	auto convert_decimal_to_hexadecimal(I n, std::string& s) -> void
	{
		s.clear();
		if (n == (I)0)
		{
			s += std::to_string((I)0);
			s.insert(s.begin(), { '0', 'x' });
		}			
		else
		{
			while (n != (I)0)
			{
				auto rem{ n % (I)16 };
				switch (rem)
				{
				case (I)10:
					s.push_back('A');
					break;
				case (I)11:
					s.push_back('B');
					break;
				case (I)12:
					s.push_back('C');
					break;
				case (I)13:
					s.push_back('D');
					break;
				case (I)14:
					s.push_back('E');
					break;
				case (I)15:
					s.push_back('F');
					break;
				default:
					s += std::to_string(rem);
					break;
				}
				n /= (I)16;
			}
			s += std::string{ "x0" };
			std::reverse(s.begin(), s.end());
		}
	}

	template<std::integral I>
	auto convert_decimal_to_hexadecimal(I n, std::wstring& s) -> void
	{
		s.clear();
		if (n == (I)0)
		{
			s += std::to_wstring((I)0);
			s.insert(s.begin(), { L'0', L'x' });
		}
		else
		{
			while (n != (I)0)
			{
				auto rem{ n % (I)16 };
				switch (rem)
				{
				case (I)10:
					s.push_back(L'A');
					break;
				case (I)11:
					s.push_back(L'B');
					break;
				case (I)12:
					s.push_back(L'C');
					break;
				case (I)13:
					s.push_back(L'D');
					break;
				case (I)14:
					s.push_back(L'E');
					break;
				case (I)15:
					s.push_back(L'F');
					break;
				default:

					s += std::to_wstring(rem);
					break;
				}
				n /= (I)16;
			}
			s += std::wstring{ L"x0" };
			std::reverse(s.begin(), s.end());
		}
	}
}
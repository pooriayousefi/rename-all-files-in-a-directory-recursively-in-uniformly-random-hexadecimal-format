
#pragma once
#include "stdh.hpp"
namespace
{
	// empty text file generator
	auto txtgen = []()
	{
		std::filesystem::path fp{ L"New Text File.txt" };
		auto counter{ 0Ui64 };

		while (std::filesystem::exists(fp))
		{
			std::filesystem::path newfp{ std::wstring{ L"New Text File (" } +
				std::to_wstring(++counter) +
				std::wstring{ L").txt" } };
			std::swap(fp, newfp);
		}

		std::wofstream ofs(fp, std::ios_base::out);
		if (!ofs) throw std::runtime_error("error: cannot open text file.");
		ofs.close();
	};
}
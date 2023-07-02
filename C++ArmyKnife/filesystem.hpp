
#pragma once
#include "dec2xxx.hpp"
namespace
{
	// read file template
	template<encoding T>
	struct read_file
	{
		inline auto operator()(std::filesystem::path file_path) -> string<T>
		{
			input_file_stream<T> ifs;
			string<T> file_contents{};
			ifs.open(file_path, std::ios::in);
			if (!ifs) throw std::runtime_error("cannot open input file in read file function.");
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			file_contents.assign(std::istreambuf_iterator<T>(ifs), std::istreambuf_iterator<T>());
			ifs.close();
			return file_contents;
		}
		inline auto operator()(input_file_stream<T>& ifs) -> string<T>
		{
			string<T> file_contents{};
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			file_contents.assign(std::istreambuf_iterator<T>(ifs), std::istreambuf_iterator<T>());
			return file_contents;
		}
	};

	// read file instantiation for textual encodings
	using read_file_in_native_narrow_encoded_string = read_file<char>;
	using read_file_in_native_wide_encoded_string = read_file<wchar_t>;
	using read_file_in_utf8_encoded_string = read_file<char8_t>;
	using read_file_in_utf16_encoded_string = read_file<char16_t>;
	using read_file_in_utf32_encoded_string = read_file<char32_t>;

	// read file specialization for binary encoding
	template<>
	struct read_file<std::byte>
	{
		inline auto operator()(std::filesystem::path file_path) -> string<std::byte>
		{
			input_file_stream<std::byte> ifs;
			string<std::byte> file_contents{};
			ifs.open(file_path, std::ios::in | std::ios::binary);
			if (!ifs) throw std::runtime_error("cannot open input file in read file function.");
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			file_contents.assign(std::istreambuf_iterator<std::byte>(ifs), std::istreambuf_iterator<std::byte>());
			ifs.close();
			return file_contents;
		}
		inline auto operator()(input_file_stream<std::byte>& ifs) -> string<std::byte>
		{
			string<std::byte> file_contents{};
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			file_contents.assign(std::istreambuf_iterator<std::byte>(ifs), std::istreambuf_iterator<std::byte>());
			return file_contents;
		}
	};

	// read file instantiation for binary encoding
	using read_file_in_binary_encoded_string = read_file<std::byte>;

	// read file line by line template
	template<encoding T>
	struct read_file_line_by_line
	{
		inline auto operator()(std::filesystem::path file_path) -> std::vector<string<T>>
		{
			input_file_stream<T> ifs;
			std::vector<string<T>> file_contents{};
			ifs.open(file_path, std::ios::in);
			if (!ifs) throw std::runtime_error("cannot open input file in read file line by line.");
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			for (string<T> line{}; std::getline(ifs, line);)
				file_contents.emplace_back(line);
			ifs.close();			
			return file_contents;
		}
		inline auto operator()(input_file_stream<T>& ifs) -> std::vector<string<T>>
		{
			std::vector<string<T>> file_contents{};
			ifs.seekg(0, std::ios::end);
			file_contents.reserve(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			for (string<T> line{}; std::getline(ifs, line);)
				file_contents.emplace_back(line);
			return file_contents;
		}
	};

	// read file line by line instantiation for textual encodings
	using read_file_line_by_line_in_native_narrow_encoded_string = read_file_line_by_line<char>;
	using read_file_line_by_line_in_native_wide_encoded_string = read_file_line_by_line<wchar_t>;
	using read_file_line_by_line_in_utf8_encoded_string = read_file_line_by_line<char8_t>;
	using read_file_line_by_line_in_utf16_encoded_string = read_file_line_by_line<char16_t>;
	using read_file_line_by_line_in_utf32_encoded_string = read_file_line_by_line<char32_t>;

	// rename a path in ascending counted fashion if the path name previously existed
	struct rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed
	{
		inline auto operator()(std::filesystem::path p) -> std::filesystem::path
		{
			const auto parent_path{ p.parent_path() };
			const auto stem{ p.stem().wstring() };
			const auto extension{ p.extension().wstring() };
			auto counter{ 0Ui64 };
			std::filesystem::path new_path{ p };
			while (std::filesystem::exists(new_path))
			{
				new_path.clear();
				new_path = parent_path /
					std::filesystem::path(stem +
						std::wstring{ L" (" } +
						std::to_wstring(++counter) +
						std::wstring{ L")" } +
						extension);
			}
			return new_path;
		}
	};

	// write file template in ascending counted rename fashion if the file is existed
	template<encoding T>
	struct write_file_in_ascending_counted_rename_fashion_if_existed
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<T> ofs;
			rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed renamer{};
			ofs.open(renamer(file_path), std::ios::out);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in ascending counted rename fashion.");
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<T>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<T> ofs;
			rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed renamer{};
			ofs.open(renamer(file_path), std::ios::out);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in ascending counted rename fashion.");
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<T>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in ascending counted rename fashion instantiation for textual encodings
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_native_narrow_encoding 
		= write_file_in_ascending_counted_rename_fashion_if_existed<char>;
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_native_wide_encoding
		= write_file_in_ascending_counted_rename_fashion_if_existed<wchar_t>;
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_utf8_encoding
		= write_file_in_ascending_counted_rename_fashion_if_existed<char8_t>;
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_utf16_encoding
		= write_file_in_ascending_counted_rename_fashion_if_existed<char16_t>;
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_utf32_encoding
		= write_file_in_ascending_counted_rename_fashion_if_existed<char32_t>;

	// write file in ascending counted rename fashion specialization for binary encoding
	template<>
	struct write_file_in_ascending_counted_rename_fashion_if_existed<std::byte>
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<std::byte> ofs;
			rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed renamer{};
			ofs.open(renamer(file_path), std::ios::out);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in ascending counted rename fashion.");
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<std::byte>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<std::byte> ofs;
			rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed renamer{};
			ofs.open(renamer(file_path), std::ios::out);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in ascending counted rename fashion.");
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<std::byte>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in ascending counted rename fashion instantiation for binary encoding
	using write_file_in_ascending_counted_rename_fashion_if_existed_in_binary_encoding
		= write_file_in_ascending_counted_rename_fashion_if_existed<std::byte>;

	// write file template in appending fashion if the file is existed
	template<encoding T>
	struct write_file_in_appending_fashion_if_existed
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<T> ofs;
			if (std::filesystem::exists(file_path))
			{
				ofs.open(file_path, std::ios::out | std::ios::app);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			else
			{
				ofs.open(file_path, std::ios::out);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<T>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<T> ofs;
			if (std::filesystem::exists(file_path))
			{
				ofs.open(file_path, std::ios::out | std::ios::app);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			else
			{
				ofs.open(file_path, std::ios::out);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<T>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in appending fashion instantiation for textual encodings
	using write_file_in_appending_fashion_if_existed_in_native_narrow_encoding
		= write_file_in_appending_fashion_if_existed<char>;
	using write_file_in_appending_fashion_if_existed_in_native_wide_encoding
		= write_file_in_appending_fashion_if_existed<wchar_t>;
	using write_file_in_appending_fashion_if_existed_in_utf8_encoding
		= write_file_in_appending_fashion_if_existed<char8_t>;
	using write_file_in_appending_fashion_if_existed_in_utf16_encoding
		= write_file_in_appending_fashion_if_existed<char16_t>;
	using write_file_in_appending_fashion_if_existed_in_utf32_encoding
		= write_file_in_appending_fashion_if_existed<char32_t>;

	// write file in appending fashion specialization for binary encoding
	template<>
	struct write_file_in_appending_fashion_if_existed<std::byte>
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<std::byte> ofs;
			if (std::filesystem::exists(file_path))
			{
				ofs.open(file_path, std::ios::out | std::ios::binary | std::ios::app);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			else
			{
				ofs.open(file_path, std::ios::out | std::ios::binary);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<std::byte>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<std::byte> ofs;
			if (std::filesystem::exists(file_path))
			{
				ofs.open(file_path, std::ios::out | std::ios::binary | std::ios::app);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			else
			{
				ofs.open(file_path, std::ios::out | std::ios::binary);
				if (!ofs)
					throw std::runtime_error("cannot open output file in write file function in appending fashion.");
			}
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<std::byte>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in appending fashion instantiation for binary encoding
	using write_file_in_appending_fashion_if_existed_in_binary_encoding
		= write_file_in_appending_fashion_if_existed<std::byte>;

	// write file template in overwriting fashion if the file is existed
	template<encoding T>
	struct write_file_in_overwriting_fashion_if_existed
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<T> ofs;
			ofs.open(file_path, std::ios::out);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in overwriting fashion.");
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<T>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<T> ofs;
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in overwriting fashion.");
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<T>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in overwriting fashion instantiation for textual encodings
	using write_file_in_overwriting_fashion_if_existed_in_native_narrow_encoding
		= write_file_in_overwriting_fashion_if_existed<char>;
	using write_file_in_overwriting_fashion_if_existed_in_native_wide_encoding
		= write_file_in_overwriting_fashion_if_existed<wchar_t>;
	using write_file_in_overwriting_fashion_if_existed_in_utf8_encoding
		= write_file_in_overwriting_fashion_if_existed<char8_t>;
	using write_file_in_overwriting_fashion_if_existed_in_utf16_encoding
		= write_file_in_overwriting_fashion_if_existed<char16_t>;
	using write_file_in_overwriting_fashion_if_existed_in_utf32_encoding
		= write_file_in_overwriting_fashion_if_existed<char32_t>;

	// write file in overwriting fashion specialization for binary encoding
	template<>
	struct write_file_in_overwriting_fashion_if_existed<std::byte>
	{
		template<typename... X>
		inline auto operator()(std::filesystem::path file_path, X&&... contents) -> void
		{
			output_file_stream<std::byte> ofs;
			ofs.open(file_path, std::ios::out | std::ios::binary);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in overwriting fashion.");
			((ofs << contents), ...);
			ofs.flush();
			ofs.close();
		}
		template<typename... X>
		inline auto operator()(output_file_stream<std::byte>& ofs, X&&... contents) -> void
		{
			((ofs << contents), ...);
		}

		template<typename Iter>
		inline auto operator()(std::filesystem::path file_path, Iter contents_begin, Iter contents_end) -> void
		{
			output_file_stream<std::byte> ofs;
			ofs.open(file_path, std::ios::out | std::ios::binary);
			if (!ofs)
				throw std::runtime_error("cannot open output file in write file function in overwriting fashion.");
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
			ofs.flush();
			ofs.close();
		}
		template<typename Iter>
		inline auto operator()(output_file_stream<std::byte>& ofs, Iter contents_begin, Iter contents_end) -> void
		{
			std::for_each(std::execution::seq,
				contents_begin,
				contents_end,
				[&](auto& content) { ofs << content; });
		}
	};

	// write file in overwriting fashion instantiation for binary encoding
	using write_file_in_overwriting_fashion_if_existed_in_binary_encoding
		= write_file_in_overwriting_fashion_if_existed<std::byte>;

	// apply an invocable on every ittem in a directory
	template<procedure<std::filesystem::path> F>
	auto apply_to_all_items_in_a_directory(F f, std::filesystem::path dirpath) -> void
	{
		if (std::filesystem::is_directory(dirpath))
			for (auto& entry : std::filesystem::directory_iterator(dirpath))
				std::invoke(f, entry.path());
	}

	// apply an invocable on every item in a directory recursively
	template<procedure<std::filesystem::path> F>
	auto apply_to_all_items_in_a_directory_recursively(F f, std::filesystem::path dirpath) -> void
	{
		if (std::filesystem::is_directory(dirpath))
		{
			std::queue<std::filesystem::path> q{};
			for (auto& entry : std::filesystem::directory_iterator(dirpath))
			{
				q.push(entry.path());
				while (!q.empty())
				{
					std::invoke(f, q.front());
					if (std::filesystem::is_directory(q.front()))
						for (auto& front_entry : std::filesystem::directory_iterator(q.front()))
							q.push(front_entry.path());
					q.pop();
				}
			}
		}
	}

	namespace asynchronous
	{
		template<procedure<std::filesystem::path> F>
		auto apply_to_all_paths(F f) -> void
		{
			auto number_of_paths{ 0Ui64 }, counter{ 1Ui64 };
			std::vector<std::filesystem::path> paths{};
			std::vector<std::future<void>> paths_futures{};
			std::cout << "How many paths do you want to apply? ";
			std::cin >> number_of_paths;
			paths.resize(number_of_paths);
			paths_futures.reserve(number_of_paths);
			std::for_each(std::execution::seq,
				paths.begin(),
				paths.end(),
				[&](auto& p)
				{
					std::cout << "\nEnter path " << counter++ << ": ";
					std::cin >> p;
				});
			for (auto& p : paths)
				paths_futures.emplace_back(std::async(std::launch::async, apply_to_all_items_in_a_directory<F>, f, p));
			for (auto& pf : paths_futures)
				pf.get();
		}

		template<procedure<std::filesystem::path> F>
		auto apply_to_all_paths_recursively(F f) -> void
		{
			auto number_of_paths{ 0Ui64 }, counter{ 1Ui64 };
			std::vector<std::filesystem::path> paths{};
			std::vector<std::future<void>> paths_futures{};
			std::cout << "How many paths do you want to apply? ";
			std::cin >> number_of_paths;
			paths.resize(number_of_paths);
			paths_futures.reserve(number_of_paths);
			std::for_each(std::execution::seq,
				paths.begin(),
				paths.end(),
				[&](auto& p)
				{
					std::cout << "\nEnter path " << counter++ << ": ";
					std::cin >> p;
				});
			for (auto& p : paths)
				paths_futures.emplace_back(std::async(std::launch::async, apply_to_all_items_in_a_directory_recursively<F>, f, p));
			for (auto& pf : paths_futures)
				pf.get();
		}
	}

	namespace synchronous
	{
		namespace buffered_copy
		{
			auto ascending_counted_rename_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				read_file_in_binary_encoded_string file_reader{};
				auto file_contents{ file_reader(from) };
				write_file_in_ascending_counted_rename_fashion_if_existed_in_binary_encoding file_writer{};
				file_writer(to, file_contents.cbegin(), file_contents.cend());
			};

			auto append_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				read_file_in_binary_encoded_string file_reader{};
				auto file_contents{ file_reader(from) };
				write_file_in_appending_fashion_if_existed_in_binary_encoding file_writer{};
				file_writer(to, file_contents.cbegin(), file_contents.cend());
			};

			auto overwrite_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				read_file_in_binary_encoded_string file_reader{};
				auto file_contents{ file_reader(from) };
				write_file_in_overwriting_fashion_if_existed_in_binary_encoding file_writer{};
				file_writer(to, file_contents.cbegin(), file_contents.cend());
			};
		}

		namespace unbuffered_copy
		{
			auto skip_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				std::filesystem::copy(from, to, std::filesystem::copy_options::skip_existing);
			};

			auto overwrite_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				std::filesystem::copy(from, to, std::filesystem::copy_options::overwrite_existing);
			};

			auto update_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				std::filesystem::copy(from, to, std::filesystem::copy_options::update_existing);
			};

			auto recursive = [](std::filesystem::path from, std::filesystem::path to)
			{
				if (std::filesystem::is_directory(from))
					std::filesystem::copy(from, to, std::filesystem::copy_options::recursive);
			};

			auto directories_only = [](std::filesystem::path from, std::filesystem::path to)
			{
				std::filesystem::copy(from, to, std::filesystem::copy_options::directories_only);
			};

			auto ascending_counted_rename_existing = [](std::filesystem::path from, std::filesystem::path to)
			{
				rename_a_path_in_ascending_counted_fashion_if_the_path_name_previously_existed renamer{};
				auto new_to = renamer(to);
				if (std::filesystem::is_directory(from))
					std::filesystem::copy(from, new_to, std::filesystem::copy_options::recursive);
				else if (std::filesystem::is_regular_file(from))
					std::filesystem::copy(from, new_to, std::filesystem::copy_options::none);
			};
		}
	}
}

#pragma once
#include "stdh.hpp"
namespace
{
	template<size_t N>
	class arena
	{
	private:
		static constexpr size_t alignment = alignof(std::max_align_t);
		static auto align_up(size_t n) noexcept->size_t { return (n + (alignment - 1)) & ~(alignment - 1); }
		auto pointer_in_buffer(const std::byte* p) const noexcept->bool { return buffer_ <= p && p <= buffer_ + N; }
		alignas(alignment) std::byte buffer_[N];
		std::byte* ptr_{};
	public:
		arena() noexcept : ptr_(buffer_) {}
		arena(const arena&) = delete;
		auto operator=(const arena&)->arena & = delete;
		auto reset() noexcept->bool { ptr_ = buffer_; }
		static constexpr auto size() noexcept->size_t { return N; }
		auto used() const noexcept->size_t { return static_cast<size_t>(ptr_ - buffer_); }
		auto allocate(size_t n) -> std::byte*
		{
			const auto aligned_n = align_up(n);
			const auto available_bytes = static_cast<decltype(aligned_n)>(buffer_ + N - ptr_);
			if (available_bytes >= aligned_n)
			{
				std::byte* r = ptr_;
				ptr_ += aligned_n;
				return r;
			}
			return static_cast<std::byte*>(::operator new(n));
		}
		auto deallocate(std::byte* p, size_t n) noexcept->void 
		{
			if (pointer_in_buffer(p))
			{
				n = align_up(n);
				if (p + n == ptr_)
				{
					ptr_ = p;
				}
			}
			else
			{
				::operator delete(p);
			}
		}
	};
}

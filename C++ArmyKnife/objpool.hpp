
#pragma once
#include "concepts.hpp"
#include "literals.hpp"
namespace
{
	// object pool
	template<default_constructible T, size_t ChunkSize>
	class object_pool
	{
	public:
		using value_type = T;
		using type = object_pool<value_type, ChunkSize>;
		using data_structure = std::vector<std::vector<value_type>>;
	private:
		alignas(value_type) data_structure object_pool_;
		size_t counter_;
		auto allocate_chunk_() -> void
		{
			object_pool_.emplace_back(std::vector<value_type>(ChunkSize, value_type()));
		}
	public:
		object_pool()
			:object_pool_{}, 
			counter_{ 0_sz }
		{
			object_pool_.reserve(2_sz * ChunkSize);
			allocate_chunk_();
		}
		object_pool(const data_structure& object_pool_, const size_t& counter_)
			:object_pool_(object_pool_), 
			counter_(counter_)
		{
			allocate_chunk_();
		}
		object_pool(const type&) = default;
		object_pool(type&&) noexcept = default;
		virtual ~object_pool() = default;
		auto operator=(const type&)->type & = default;
		auto operator=(type&&) noexcept -> type & = default;
		bool operator==(const object_pool& other) const = default;
		auto acquire_object() -> std::reference_wrapper<value_type>
		{
			if (counter_ == ChunkSize)
			{
				allocate_chunk_();
				counter_ = 0_sz;
			}
			return std::ref(object_pool_.back()[counter_++]);
		}
	};
}
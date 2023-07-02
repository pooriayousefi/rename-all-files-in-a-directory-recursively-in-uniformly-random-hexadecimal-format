
#pragma once
#include "stdh.hpp"
namespace
{
	class observable;

	class observer
	{
	public:
		virtual ~observer() = default;
		virtual void update(observable&) = 0;
	};

	class observable
	{
	public:
		virtual ~observable() = default;
		void attach(observer& o) { observers_.push_back(&o); }
		void detach(observer& o) { observers_.erase(std::remove(observers_.begin(), observers_.end(), &o)); }
		void notify() { for (auto* o : observers_) o->update(*this); }

	private:
		std::vector<observer*> observers_;
	};
}
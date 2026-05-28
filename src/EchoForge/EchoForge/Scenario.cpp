#include "Scenario.h"

using namespace ef;


Scenario::Scenario(std::chrono::milliseconds tick_rate): tick_rate_(tick_rate){}

Scenario::~Scenario() { stop(); }

void Scenario::add_event(Action action, Condition condition) { events_.push_back({ std::move(condition), std::move(action) }); }

void Scenario::clearEvent() { events_.clear(); }

void Scenario::stop() {
	running_ = false;
	if (thread_.joinable()) thread_.join();
}

void Scenario::start() {
	running_ = true;
	thread_ = std::thread([this]
		{
			while (running_)
			{
				for (auto& event : events_) {
					if (event.condition()) { event.action(); }
				}
				std::this_thread::sleep_for(tick_rate_);
			}
		}
	);
}
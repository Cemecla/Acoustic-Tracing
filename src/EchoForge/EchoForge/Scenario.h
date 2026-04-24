#pragma once

#include <Vector.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>



namespace ef {

	class Scenario
	{
	public:
		explicit Scenario(std::chrono::milliseconds tick_rate = std::chrono::milliseconds(100));
		~Scenario();

		using Condition = std::function<bool()>;
		using Action = std::function<void()>;

		void update(const float delta);

		void add_event(Action action,Condition condition);

		void stop();

		void start();

	private:

		struct Event
		{
			Condition condition;
			Action action;
		};

	

		std::vector<Event> events_;
		std::thread thread_;
		std::atomic<bool> running_{ false };
		std::chrono::microseconds tick_rate_;
	};
}
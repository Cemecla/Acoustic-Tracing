#pragma once

namespace ef {




	class Scenario
	{
	public:
		Scenario();
		~Scenario();

	private:

		enum class ConditionType
		{
			Positional,
			Timer
		};
		

		class Condition
		{
		public:
			Condition(ConditionType type)
			{
			}

		private:
			ConditionType m_type;


		};





	};

	Scenario::Scenario()
	{
	}

	Scenario::~Scenario()
	{
	}
}
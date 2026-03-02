#pragma once
#include <tuple>
#include <Memory>


using namespace std;

class Avatar
{
	public:
		/**
		*	Avatar constructor
		*	The Avatar object need a starting position and angles positions to initialize it's nested Head object (See Head class)
		*   param position: tuple(x, y, z)
		*	param angles:   tuple(x, y, z)
		*/
		Avatar(tuple<float, float, float> position, tuple<float, float, float> angles);
		tuple<float, float, float> getHeadAngles();
		void rotateHead(tuple<float, float, float> mods);
		void move(tuple<float, float, float> mods);
		void moveTo(tuple<float, float, float> newPos);
		tuple<float, float, float> getPosition();
	
	private:
		class Head {
			public:
				/**
				*	Head constructor
				*	The Head object need starting angles positions
				*	param angles: tuple(x, y, z)
				*/
				Head(tuple<float, float, float> angles);

				void rotate(tuple<float, float, float> mods);
				tuple<float, float, float> getAngles();

			private:
				// Variables
				tuple<float, float, float> m_angles;

		
		};
		unique_ptr<Head> m_head;
		tuple<float, float, float> m_position;
};

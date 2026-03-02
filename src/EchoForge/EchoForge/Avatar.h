#pragma once
#include <tuple>
#include <Memory>


using namespace std;

class Avatar
{
	public:
		/**
		*	Main constructor
		*	The Avatar object need a starting position and angles positions to initialize it's nested Head object (See Head class)
		*   param position: tuple(x, y, z)
		*	param angles:   tuple(x, y, z)
		*/
		Avatar(tuple<float, float, float> position, tuple<float, float, float> angles);

		/**
		*	This method return the current x,y,z angle values of the Head
		*	return: tuple(x, y, z)
		*/
		tuple<float, float, float> getHeadAngles();

		/**
		*	This method modify the current Head angles by the given tuple
		*   param mods: tuple(x, y, z)
		*/
		void rotateHead(tuple<float, float, float> mods);

		/**
		*	This method modify the current position by the given tuple
		*   param mods: tuple(x, y, z)
		*/
		void move(tuple<float, float, float> mods);

		/**
		* This method change the current position to the given tuple
		* param newPos: tuple(x, y, z)
		*/
		void moveTo(tuple<float, float, float> newPos);

		/**
		*	This method return the current position of the Avatar
		*   return: tuple(x, y, z)
		*/
		tuple<float, float, float> getPosition();
	
	private:
		class Head {
			public:
				/**
				*	Main constructor
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

#pragma once
#include <tuple>
#include <Memory>

using namespace std;

#ifndef __Avatar__
	#define __Avatar__
	class Avatar
	{
		public:
			/**
			*	Main constructor
			*	desc: The Avatar object need a starting position and angles positions to initialize it's nested Head object (See Head class)
			*   param position: tuple(x, y, z)
			*	param angles:   tuple(x, y, z)
			*/
			Avatar(tuple<float, float, float> position, tuple<float, float, float> angles);
	
			/**
			*	desc: This method return the current x,y,z angle values of the Head
			*	return: tuple(x, y, z)
			*/
			tuple<float, float, float> getHeadAngles();
	
			/**
			*	desc: This method modify the current Head angles by the given tuple
			*   param mods: tuple(x, y, z)
			*/
			void rotateHead(tuple<float, float, float> mods);
	
			/**
			*	desc: This method modify the current position by the given tuple
			*   param mods: tuple(x, y, z)
			*/
			void move(tuple<float, float, float> mods);
	
			/**
			*	desc: This method change the current position to the given tuple
			*	param newPos: tuple(x, y, z)
			*/
			void moveTo(tuple<float, float, float> newPos);
	
			/**
			*	desc: This method return the current position of the Avatar
			*   return: tuple(x, y, z)
			*/
			tuple<float, float, float> getPosition();
		
		private:
			class Head {
				public:
					/**
					*	Main constructor
					*	desc: The Head object need starting angles positions
					*	param angles: tuple(x, y, z)
					*/
					Head(tuple<float, float, float> angles);
	
					/**
					*	desc: This method rotate the current Head angles from the given values
					*   mods: tuple(x, y, z)
					*/
					void rotate(tuple<float, float, float> mods);

					/**
					*	desc: This method return the angles of the Head
					*   return: tuple(x, y, z)
					*/
					tuple<float, float, float> getAngles();
	
				private:
					// Variables
					tuple<float, float, float> m_angles;
	
			
			};

			// Variables
			unique_ptr<Head> m_head;
			tuple<float, float, float> m_position;
	};

#endif
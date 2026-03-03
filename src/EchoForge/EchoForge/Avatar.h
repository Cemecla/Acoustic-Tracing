#pragma once
#include <tuple>
#include <memory>
#include "Vector.h"

#ifndef AVATAR_H
	#define AVATAR_H
	class Avatar
	{
		public:
			/**
			*	Main constructor
			*	desc: The Avatar object need a starting position and angles positions to initialize it's nested Head object (See Head class)
			*   param position: tuple(x, y, z)
			*	param angles:   tuple(x, y, z)
			*/
			Avatar(Vector::Vector3 position, Vector::Vector3 angles);
	
			/**
			*	desc: This method return the current x,y,z angle values of the Head
			*	return: tuple(x, y, z)
			*/
			Vector::Vector3 getHeadAngles();
	
			/**
			*	desc: This method modify the current Head angles by the given tuple
			*   param mods: tuple(x, y, z)
			*/
			void rotateHead(Vector::Vector3 mods);
	
			/**
			*	desc: This method modify the current position by the given tuple
			*   param mods: tuple(x, y, z)
			*/
			void move(Vector::Vector3 mods);
	
			/**
			*	desc: This method change the current position to the given tuple
			*	param newPos: tuple(x, y, z)
			*/
			void moveTo(Vector::Vector3 newPos);
	
			/**
			*	desc: This method return the current position of the Avatar
			*   return: tuple(x, y, z)
			*/
			Vector::Vector3 getPosition();
		
		private:
			class Head {
				public:
					/**
					*	Main constructor
					*	desc: The Head object need starting angles positions
					*	param angles: tuple(x, y, z)
					*/
					Head(Vector::Vector3 angles);
	
					/**
					*	desc: This method rotate the current Head angles from the given values
					*   mods: tuple(x, y, z)
					*/
					void rotate(Vector::Vector3 mods);

					/**
					*	desc: This method return the angles of the Head
					*   return: tuple(x, y, z)
					*/
					Vector::Vector3 getAngles();
	
				private:
					// Variables
					Vector::Vector3 m_angles;
			
			};

			// Variables
			Head m_head;
			Vector::Vector3 m_position;
	};

#endif
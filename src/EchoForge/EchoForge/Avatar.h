#pragma once
#include <memory>
#include "Vector.h"

#ifndef AVATAR_H
	#define AVATAR_H

	class Avatar {
		public:
			/**
			*	Main constructor
			*	desc: The Avatar object need a starting position and angles positions to initialize it's nested Head object (See Head class)
			*   param position: Vector3(x, y, z)
			*	param angles:   Vector3(x, y, z)
			*/
			Avatar(EF::Vector3 position, EF::Vector3 angles);
	
			/**
			*	desc: This method return the current x,y,z angle values of the Head
			*	return: Vector3(x, y, z)
			*/
			const EF::Vector3& getHeadAngles() const;
	
			/**
			*	desc: This method modify the current Head angles by the given Vector3
			*   param mods: Vector3(x, y, z)
			*/
			void rotateHead(const EF::Vector3& mods);
	
			/**
			*	desc: This method modify the current position by the given Vector3
			*   param mods: Vector3(x, y, z)
			*/
			void move(const EF::Vector3& mods);
	
			/**
			*	desc: This method change the current position to the given Vector3
			*	param newPos: Vector3(x, y, z)
			*/
			void moveTo(const EF::Vector3& newPos);
	
			/**
			*	desc: This method return the current position of the Avatar
			*   return: Vector3(x, y, z)
			*/
			const EF::Vector3& getPosition() const;
		
		private:
			class Head {
				public:
					/**
					*	Main constructor
					*	desc: The Head object need starting angles positions
					*	param angles: Vector3(x, y, z)
					*/
					Head(EF::Vector3 angles);
	
					/**
					*	desc: This method rotate the current Head angles from the given values
					*   mods: Vector3(x, y, z)
					*/
					void rotate(const EF::Vector3& mods);

					/**
					*	desc: This method return the angles of the Head
					*   return: Vector3(x, y, z)
					*/
					const EF::Vector3& getAngles() const;
	
				private:
					// Variables
					EF::Vector3 m_angles;
			
			};

			// Variables
			Head m_head;
			EF::Vector3 m_position;
	};

#endif
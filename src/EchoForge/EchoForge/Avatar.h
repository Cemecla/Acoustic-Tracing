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
			Avatar(ef::Vector3 position, ef::Vector3 angles);
	
			/**
			*	desc: This method return the current x,y,z angle values of the Head
			*	return: Vector3(x, y, z)
			*/
			const ef::Vector3& getHeadAngles() const;
	
			/**
			*	desc: This method modify the current Head angles by the given Vector3
			*   param mods: Vector3(x, y, z)
			*/
			void rotateHead(const ef::Vector3& mods);
	
			/**
			*	desc: This method modify the current position by the given Vector3
			*   param mods: Vector3(x, y, z)
			*/
			void move(const ef::Vector3& mods);
	
			/**
			*	desc: This method change the current position to the given Vector3
			*	param newPos: Vector3(x, y, z)
			*/
			void moveTo(const ef::Vector3& newPos);
	
			/**
			*	desc: This method return the current position of the Avatar
			*   return: Vector3(x, y, z)
			*/
			const ef::Vector3& getPosition() const;
		
		private:
			class Head {
				public:
					/**
					*	Main constructor
					*	desc: The Head object need starting angles positions
					*	param angles: Vector3(x, y, z)
					*/
					Head(ef::Vector3 angles);
	
					/**
					*	desc: This method rotate the current Head angles from the given values
					*   mods: Vector3(x, y, z)
					*/
					void rotate(const ef::Vector3& mods);

					/**
					*	desc: This method return the angles of the Head
					*   return: Vector3(x, y, z)
					*/
					const ef::Vector3& getAngles() const;
	
				private:
					// Variables
					ef::Vector3 m_angles;
			};

			// Variables
			Head m_head;
			ef::Vector3 m_position;
	};

#endif
#pragma once
#include <memory>
#include "Vector.h"

#ifndef AVATAR_H
	#define AVATAR_H
	namespace ef {
		class Avatar {
			public:
				/**
				*	@summary Constructs an Avatar with default position and head orientation.
				*/
				Avatar();
	
				/**
				*	@summary Constructs an Avatar with a starting position and initial head angles.
				*	@param position Initial world position as Vector3(x, y, z).
				*	@param angles Initial head angles as Vector3(x, y, z).
				*/
				Avatar(Vector3F position, Vector3F angles);
	
				/**
				*	@summary Returns the current head rotation angles.
				*	@return Constant reference to Vector3(x, y, z) representing head angles.
				*/
				const Vector3F& getHeadAngles() const;
	
				/**
				*	@summary Rotates the head by adding the specified angle modifications.
				*	@param mods Angle modifications as Vector3(x, y, z).
				*/
				void rotateHead(const Vector3F& mods);
	
				/**
				*	@summary Moves the Avatar by adding the specified offset to its current position.
				*	@param mods Movement offset as Vector3(x, y, z).
				*/
				void move(const Vector3F& mods);
	
				/**
				*	@summary Sets the Avatar's position to the specified coordinates.
				*	@param newPos New absolute position as Vector3(x, y, z).
				*/
				void moveTo(const Vector3F& newPos);
	
				/**
				*	@summary Returns the current world position of the Avatar.
				*	@return Constant reference to Vector3(x, y, z) representing position.
				*/
				const Vector3F& getPosition() const;
	
			private:
				class Head {
				public:
					/**
					*	@summary Constructs a Head with the specified initial rotation angles.
					*	@param angles Initial angles as Vector3(x, y, z).
					*/
					Head(Vector3F angles);
	
					/**
					*	@summary Rotates the head by adding the specified angle modifications.
					*	@param mods Angle modifications as Vector3(x, y, z).
					*/
					void rotate(const Vector3F& mods);
	
					/**
					*	@summary Returns the current rotation angles of the head.
					*	@return Constant reference to Vector3(x, y, z) representing angles.
					*/
					const Vector3F& getAngles() const;
	
				private:
					// Variables
					Vector3F m_angles;
				};
	
				// Variables
				Head m_head;
				Vector3F m_position;
		};
	}
#endif
#pragma once

namespace Vector {
	struct Vector2
	{
		float x{};
		float y{};

		/**
		*	Main constructor
		*	desc: 
		*   param x: float
		*	param y: float
		*/
		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		/**
		*   desc: Vector2 need two value x, y
		*   param v: Vector2&
		*/
		Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}

		/**
		*	desc: Add two Vector2 to create a new Vector2
		*   param v: Vector2&
		*/
		void operator+=(const Vector2& v) {
			x += v.x;
			y += v.y;
		}
	};

	struct Vector3
	{
		float x{};
		float y{};
		float z{};

		/**
		*	Main constructor
		*	desc: Vector3 need three value x, y, z 
		*   param x: float
		*	param y: float
		*   param z: float
		*/
		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/**
		*	desc: Add two Vector3 to create a new Vector3
		*   param v: Vector3&
		*/
		Vector3 operator+(const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		/**
		*	desc: Add a Vector3 to this Vector3 struct
		*   param v: Vector3&
		*/
		void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}
	};
}

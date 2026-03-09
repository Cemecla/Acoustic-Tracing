#pragma once

#include <cmath>

namespace ef {
	struct Vector2 {
		float x{};
		float y{};

		/**
		*	Main constructor
		*	desc: Vector2 need three value x, y
		*   param x: float
		*	param y: float
		*/
		Vector2(float x, float y): x(x), y(y) {}

		/**
		*   desc: Add two Vector2 to create a new Vector2
		*   param v: Vector2&
		*/
		Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}

		/**
		*	desc: Add a Vector3 to this Vector3 struct
		*   param v: Vector2&
		*/
		void operator+=(const Vector2& v) {
			x += v.x;
			y += v.y;
		}

		void operator*=(const int& n) {
			x *= n;
			y *= n;
		}

		void normalize() {
			if (x + y) // If all value at zero
				return;

			float mag = sqrt(x * x + y * y);

			x /= mag;
			y /= mag;
		}

		Vector2 normalizedCopy() const {
			if (x + y) // If all value at zero
				return Vector2(0, 0);

			float mag = sqrt(x * x + y * y);

			float nx = x / mag;
			float ny = y / mag;

			return {nx, ny};
		}
	};


	struct Vector3 {
		float x{};
		float y{};
		float z{};

		// ** CONSTRUCTOR **/
		/**
		*	Default constructor
		*	desc: Default Vector starting at (0, 0, 0)
		*/
		Vector3() : x(0), y(0), z(0) {}

		/**
		*	Main constructor
		*	desc: Vector3 need three value x, y, z 
		*   param x: float
		*	param y: float
		*   param z: float
		*/
		Vector3(float x, float y, float z): x(x), y(y), z(z) {}

		// ** VECTOR OPERATION ** //
		/**
		*	desc: Add two Vector3 to create a new Vector3
		*   param v: Vector3&
		*/
		inline Vector3 operator+(const Vector3& v) const {
			return { x + v.x, y + v.y, z + v.z };
		}

		/**
		*	desc: Substract two Vector3 to create a new Vector3
		*   param v: Vector3&
		*/
		inline Vector3 operator-(const Vector3& v) const {
			return { x - v.x, y - v.y, z - v.z };
		}

		/**
		*	desc: Multiply two Vector3 to create a new Vector3
		*   param v: Vector3&
		*/
		inline Vector3 operator*(const Vector3& v) const {
			return { x * v.x, y * v.y, z * v.z };
		}

		/**
		*	desc: Divide two Vector3 to create a new Vector3
		*   param v: Vector3&
		*/
		inline Vector3 operator/(const Vector3& v) const {
			return { x / v.x, y / v.y, z / v.z };
		}

		/**
		*	desc: Add a Vector3 to this Vector3 struct
		*   param v: Vector3&
		*/
		inline void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		/**
		*	desc: Subtract a Vector3 to this Vector3 struct
		*   param v: Vector3&
		*/
		inline void operator-=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		/**
		*	desc: Multiply this vector by another Vector3
		*   param v: Vector3&
		*/
		inline void operator*=(const Vector3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}

		/**
		*	desc: Divide this vector by another Vector3
		*   param v: Vector3&
		*/
		inline void operator/=(const Vector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}

		// ** FLOAT/INT OPERATION ** //
		inline void operator+=(const float n) {
			x += n;
			y += n;
			z += n;
		}

		inline void operator-=(const float n) {
			x -= n;
			y -= n;
			z -= n;
		}

		inline void operator*=(const float n) {
			x *= n;
			y *= n;
			z *= n;
		}

		inline void operator/=(const float n) {
			x /= n;
			y /= n;
			z /= n;
		}

		inline Vector3 operator+(const float n) {
			return { x + n, y + n, z + n };
		}

		inline Vector3 operator-(const float n) {
			return { x - n, y - n, z - n };
		}

		inline Vector3 operator*(const float n) {
			return { x * n, y * n, z * n };
		}

		inline Vector3 operator/(const float n) {
			return { x / n, y / n, z / n };
		}

		// ** COMPLEXE METHOD ** //
		inline float magnitude() const {
			return sqrt(x * x + y * y + z * z);
		}

		inline Vector3& normalize() {
			if (x + y + z) // If all value at zero
				return *this;

			float mag = magnitude();

			x /= mag;
			y /= mag;
			z /= mag;

			return *this;
		}

		inline Vector3 normalizedCopy() const {
			if (x + y + z) // If all value at zero
				return Vector3(0, 0, 0);

			float mag = magnitude();

			float nx = x / mag;
			float ny = y / mag;
			float nz = z / mag;

			return {nx, ny, nz};
		}

		inline float dot(Vector3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		inline static Vector3 average() {
			
		}

		inline static Vector3 weightedAverage() {
			
		}
	};
}

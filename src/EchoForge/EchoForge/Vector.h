#pragma once

#include <cmath>

// temporay -> prompt : Don't change the code and don't comment on it. I wish to modify/add <summary> comment for all the function. Can you modify and add those comments

namespace ef {
	struct Vector2 {
		float x{};
		float y{};

		/**
		*	@summary Constructs a 2D vector with the specified x and y components.
		*	@param x The horizontal component.
		*	@param y The vertical component.
		*/
		Vector2(float x, float y): x(x), y(y) {}

		/**
		*   @summary Returns a new Vector2 representing the sum of this vector and another.
		*   @param v The vector to add.
		*   @return A new Vector2 containing the component-wise addition result.
		*/
		Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}

		/**
		*	@summary Adds another Vector2 to this vector in place.
		*   @param v The vector to add.
		*/
		void operator+=(const Vector2& v) {
			x += v.x;
			y += v.y;
		}

		/**
		*	@summary Multiplies both components of this vector by an integer scalar in place.
		*	@param n The scalar multiplier.
		*/
		void operator*=(const int& n) {
			x *= n;
			y *= n;
		}

		/**
		*	@summary Normalizes this vector in place to have a magnitude of 1 when possible.
		*/
		void normalize() {
			if (x + y) // If all value at zero
				return;

			float mag = sqrt(x * x + y * y);

			x /= mag;
			y /= mag;
		}

		/**
		*	@summary Returns a normalized copy of this vector.
		*	@return A new Vector2 with magnitude 1 when possible, otherwise a zero vector.
		*/
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
		*	@summary Constructs a Vector3 initialized to (0, 0, 0).
		*/
		Vector3() : x(0), y(0), z(0) {}

		/**
		*	@summary Constructs a 3D vector with the specified x, y, and z components.
		*   @param x The x component.
		*	@param y The y component.
		*   @param z The z component.
		*/
		Vector3(float x, float y, float z): x(x), y(y), z(z) {}

		// ** VECTOR OPERATION ** //
		/**
		*	@summary Returns a new Vector3 representing the sum of this vector and another.
		*   @param v The vector to add.
		*	@return A new Vector3 containing the component-wise addition result.
		*/
		inline Vector3 operator+(const Vector3& v) const {
			return { x + v.x, y + v.y, z + v.z };
		}

		/**
		*	@summary Returns a new Vector3 representing the difference between this vector and another.
		*   @param v The vector to subtract.
		*	@return A new Vector3 containing the component-wise subtraction result.
		*/
		inline Vector3 operator-(const Vector3& v) const {
			return { x - v.x, y - v.y, z - v.z };
		}

		/**
		*	@summary Returns a new Vector3 resulting from component-wise multiplication with another vector.
		*   @param v The vector to multiply with.
		*	@return A new Vector3 containing the component-wise multiplication result.
		*/
		inline Vector3 operator*(const Vector3& v) const {
			return { x * v.x, y * v.y, z * v.z };
		}

		/**
		*	@summary Returns a new Vector3 resulting from component-wise division by another vector.
		*   @param v The vector to divide by.
		*	@return A new Vector3 containing the component-wise division result.
		*/
		inline Vector3 operator/(const Vector3& v) const {
			return { x / v.x, y / v.y, z / v.z };
		}

		/**
		*	@summary Adds another Vector3 to this vector in place.
		*   @param v The vector to add.
		*/
		inline void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		/**
		*	@summary Subtracts another Vector3 from this vector in place.
		*   @param v The vector to subtract.
		*/
		inline void operator-=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		/**
		*	@summary Multiplies this vector component-wise by another Vector3 in place.
		*   @param v The vector to multiply with.
		*/
		inline void operator*=(const Vector3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}

		/**
		*	@summary Divides this vector component-wise by another Vector3 in place.
		*   @param v The vector to divide by.
		*/
		inline void operator/=(const Vector3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}

		// ** FLOAT/INT OPERATION ** //
		/**
		*	@summary Adds a scalar value to each component of this vector in place.
		*	@param n The scalar value to add.
		*/
		inline void operator+=(const float n) {
			x += n;
			y += n;
			z += n;
		}

		/**
		*	@summary Subtracts a scalar value from each component of this vector in place.
		*	@param n The scalar value to subtract.
		*/
		inline void operator-=(const float n) {
			x -= n;
			y -= n;
			z -= n;
		}

		/**
		*	@summary Multiplies each component of this vector by a scalar in place.
		*	@param n The scalar multiplier.
		*/
		inline void operator*=(const float n) {
			x *= n;
			y *= n;
			z *= n;
		}

		/**
		*	@summary Divides each component of this vector by a scalar in place.
		*	@param n The scalar divisor.
		*/
		inline void operator/=(const float n) {
			x /= n;
			y /= n;
			z /= n;
		}

		/**
		*	@summary Returns a new Vector3 with a scalar added to each component.
		*	@param n The scalar value to add.
		*	@return A new Vector3 containing the result.
		*/
		inline Vector3 operator+(const float n) {
			return { x + n, y + n, z + n };
		}

		/**
		*	@summary Returns a new Vector3 with a scalar subtracted from each component.
		*	@param n The scalar value to subtract.
		*	@return A new Vector3 containing the result.
		*/
		inline Vector3 operator-(const float n) {
			return { x - n, y - n, z - n };
		}

		/**
		*	@summary Returns a new Vector3 with each component multiplied by a scalar.
		*	@param n The scalar multiplier.
		*	@return A new Vector3 containing the result.
		*/
		inline Vector3 operator*(const float n) {
			return { x * n, y * n, z * n };
		}

		/**
		*	@summary Returns a new Vector3 with each component divided by a scalar.
		*	@param n The scalar divisor.
		*	@return A new Vector3 containing the result.
		*/
		inline Vector3 operator/(const float n) {
			return { x / n, y / n, z / n };
		}

		// ** COMPLEXE METHOD ** //
		/**
		*	@summary Computes and returns the magnitude (length) of this vector.
		*	@return The Euclidean length of the vector.
		*/
		inline float magnitude() const {
			return sqrt(x * x + y * y + z * z);
		}

		/**
		*	@summary Normalizes this vector in place to have a magnitude of 1 when possible.
		*	@return A reference to this normalized vector.
		*/
		inline Vector3& normalize() {
			if (!(x + y + z)) // If all value at zero
				return *this;

			float mag = magnitude();

			x /= mag;
			y /= mag;
			z /= mag;

			return *this;
		}

		/**
		*	@summary Returns a normalized copy of this vector.
		*	@return A new Vector3 with magnitude 1 when possible, otherwise a zero vector.
		*/
		inline Vector3 normalizedCopy() const {
			if (!(x + y + z)) // If all value at zero
				return Vector3(0, 0, 0);

			float mag = magnitude();

			float nx = x / mag;
			float ny = y / mag;
			float nz = z / mag;

			return {nx, ny, nz};
		}

		/**
		*	@summary Computes the dot product between this vector and another.
		*	@param v The other vector.
		*	@return The scalar dot product result.
		*/
		inline float dot(Vector3& v) const {
			return x * v.x + y * v.y + z * v.z;
		}

		/**
		*	@summary Computes the average of multiple Vector3 values.
		*	@return A Vector3 representing the average.
		*/
		inline static Vector3 average() {
			
		}

		/**
		*	@summary Computes a weighted average of multiple Vector3 values.
		*	@return A Vector3 representing the weighted average.
		*/
		inline static Vector3 weightedAverage() {
			
		}
	};
}
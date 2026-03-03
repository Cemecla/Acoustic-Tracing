#pragma once

namespace Vector {
	struct Vector2
	{
		float x{};
		float y{};

		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2 operator+(const Vector2& v) {
			return Vector2(x + v.x, y + v.y);
		}

		void operator+=(const Vector2& v) {
			x += v.x;
			y += v.y;
		}
	};

	struct Vector3
	{
		// Note {} braces mean initialize to 0.0f and not the value that was there (Delete this note later)
		float x{};
		float y{};
		float z{};

		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3 operator+(const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}
	};
}

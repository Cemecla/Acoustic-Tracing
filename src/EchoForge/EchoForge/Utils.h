#pragma once

enum class Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

#ifndef ERROR_H
#define ERROR_H
enum Error {
	UNDEFINED = 0,
	FILE_NOT_FOUND = 2,
	PATH_NOT_FOUND = 3,
	ACCESS_DENIED = 5,
	ERROR_INVALID_DATA = 13,
	TYPE = 200,
	PARAMETER_OUT_OF_BOUNDS = 5913
};

#endif

struct ColorRGBA
{
	uint8_t r{};
	uint8_t g{};
	uint8_t b{};
	uint8_t a{};
	
	ColorRGBA() : r(0), g(0), b(0), a(0) {

	}

	ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {
	
	}
};


struct Inputs {
	bool forward = false;
	bool backward = false;
	bool left = false;
	bool right = false;
	bool rotLeft = false;
	bool rotRight = false;
};
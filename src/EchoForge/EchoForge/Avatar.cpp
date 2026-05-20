#include "Avatar.h"

using namespace std;
using namespace ef;

// AVATAR SECTION
Avatar::Avatar() : m_head(Vector3F(0,0,0)), m_position(Vector3F(0, 0, 0)), m_speed(200.0) {

}

Avatar::Avatar(Vector3F position, Vector3F angles): m_head(angles), m_position(position), m_speed(200.0) {

}

Avatar::Avatar(Vector3F position, Vector3F angles, float speed): m_head(angles), m_position(position), m_speed(speed) {

}

const Vector3F& Avatar::getHeadAngles() const {
	return m_head.getAngles();
}

void Avatar::rotateHead(const Vector3F& mods) {
	m_head.rotate(mods);
}

const Vector3F& Avatar::getPosition() const {
	return m_position;
}

const Vector3F& Avatar::move(const Vector3F& mods) {
	m_position += mods;
	return m_position;
}

void Avatar::moveTo(const Vector3F& newPos) {
	m_position = newPos;
}

float Avatar::getSpeed() const {
	return m_speed;

}

float Avatar::setSpeed(float speed) {
	m_speed = speed;
	return m_speed;
}

const float Avatar::getRotationSpeed() const {
	return m_head.getRotationSpeed();
}

void Avatar::setRotationSpeed(float rspeed) {
	m_head.setRotationSpeed(rspeed);
}

// HEAD SECTION
Avatar::Head::Head(Vector3F angles): m_angles(angles) {

}

void Avatar::Head::rotate(const Vector3F& mods) {
	m_angles += mods;
}

const float Avatar::Head::getRotationSpeed() const {
	return m_rotateSpeed;
}

void Avatar::Head::setRotationSpeed(float rspeed) {
	m_rotateSpeed = rspeed;
}

const Vector3F& Avatar::Head::getAngles() const {
	return m_angles;
}
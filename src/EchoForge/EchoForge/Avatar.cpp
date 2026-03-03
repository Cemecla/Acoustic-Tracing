#include "Avatar.h"

using namespace std;

// AVATAR SECTION
Avatar::Avatar(Vector::Vector3 position, Vector::Vector3 angles): m_head(angles), m_position(position)
{

}

Vector::Vector3 Avatar::getHeadAngles() {
	return m_head.getAngles();
}

void Avatar::rotateHead(Vector::Vector3 mods) {
	m_head.rotate(mods);
}

Vector::Vector3 Avatar::getPosition() {
	return m_position;
}

void Avatar::move(Vector::Vector3 mods) {
	// m_position += mods;
}

void Avatar::moveTo(Vector::Vector3 newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(Vector::Vector3 angles): m_angles(angles) {

}

void Avatar::Head::rotate(Vector::Vector3 mods) {
	// m_angles += mods;
}

Vector::Vector3 Avatar::Head::getAngles() {
	return m_angles;
}
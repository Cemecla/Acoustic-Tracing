#include "Avatar.h"

using namespace std;
using namespace ef;

// AVATAR SECTION
Avatar::Avatar() : m_head(Vector3(0,0,0)), m_position(Vector3(0, 0, 0)) {

}

Avatar::Avatar(Vector3 position, Vector3 angles): m_head(angles), m_position(position) {

}

const Vector3& Avatar::getHeadAngles() const {
	return m_head.getAngles();
}

void Avatar::rotateHead(const Vector3& mods) {
	m_head.rotate(mods);
}

const Vector3& Avatar::getPosition() const {
	return m_position;
}

void Avatar::move(const Vector3& mods) {
	m_position += mods;
}

void Avatar::moveTo(const Vector3& newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(Vector3 angles): m_angles(angles) {

}

void Avatar::Head::rotate(const Vector3& mods) {
	m_angles += mods;
}

const Vector3& Avatar::Head::getAngles() const {
	return m_angles;
}
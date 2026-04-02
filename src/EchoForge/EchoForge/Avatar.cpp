#include "Avatar.h"

using namespace std;
using namespace ef;

// AVATAR SECTION
Avatar::Avatar() : m_head(Vector3F(0,0,0)), m_position(Vector3F(0, 0, 0)) {

}

Avatar::Avatar(Vector3F position, Vector3F angles): m_head(angles), m_position(position) {

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

void Avatar::move(const Vector3F& mods) {
	m_position += mods;
}

void Avatar::moveTo(const Vector3F& newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(Vector3F angles): m_angles(angles) {

}

void Avatar::Head::rotate(const Vector3F& mods) {
	m_angles += mods;
}

const Vector3F& Avatar::Head::getAngles() const {
	return m_angles;
}
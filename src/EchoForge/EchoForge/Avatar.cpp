#include "Avatar.h"

// AVATAR SECTION
Avatar::Avatar(tuple<float, float, float> position, tuple<float, float, float> angles): m_head(make_unique<Head>(angles)), m_position(position)
{

}

tuple<float, float, float> Avatar::getHeadAngles() {
	return m_head->getAngles();
}

void Avatar::rotateHead(tuple<float, float, float> mods) {
	m_head->rotate(mods);
}

tuple<float, float, float> Avatar::getPosition() {
	return m_position;
}

void Avatar::move(tuple<float, float, float> mods) {
	// m_position += mods;
}

void Avatar::moveTo(tuple<float, float, float> newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(tuple<float, float, float> angles): m_angles(angles) {

}

void Avatar::Head::rotate(tuple<float, float, float> mods) {
	// m_angles += mods;
}

tuple<float, float, float> Avatar::Head::getAngles() {
	return m_angles;
}
#include <QQmlApplicationEngine>

#include "EFView.h"
#include "EFController.h"

EFView::EFView(EFController* parent) : m_parent(parent), m_qtEngine(new QQmlApplicationEngine)
{
	m_qtEngine->load(QUrl(QStringLiteral("qrc:/qt/qml/echoforge/main.qml")));
}

EFView::~EFView()
{
}

void EFView::update() {

}
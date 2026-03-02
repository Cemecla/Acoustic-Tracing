#include "EFView.h"
#include "EFController.h"
#include <QQmlApplicationEngine>

EFView::EFView(EFController* parent) : parent_(parent), qtEngine_(new QQmlApplicationEngine)
{
	qtEngine_->load(QUrl(QStringLiteral("qrc:/qt/qml/echoforge/main.qml")));
}

EFView::~EFView()
{
}
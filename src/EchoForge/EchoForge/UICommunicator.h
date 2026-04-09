#pragma once
#include <QObject>
#include <string>
#include "Vector.h"

class EFView;

/// <summary>
/// Testing
/// https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html
/// </summary>
class UICommunicator: public QObject {
	Q_OBJECT
	public:
		UICommunicator(EFView* view);
		UICommunicator(EFView* view, ef::Vector3F pos, ef::Vector3F angles);
		// READ -> Getter WRITE -> Setter NOTIFY -> SIGNAL
		Q_PROPERTY(ef::Vector3F avatarPos READ avatarPos WRITE setAvatarPos NOTIFY avatarPosChanged);
		Q_PROPERTY(ef::Vector3F headAngles READ headAngles WRITE setHeadAngles NOTIFY headAnglesChanged);


		const ef::Vector3F& avatarPos() const;
		const ef::Vector3F& headAngles() const;

		void setAvatarPos(ef::Vector3F& v);
		void setHeadAngles(ef::Vector3F& v);

		Q_INVOKABLE void startSimulation();
		Q_INVOKABLE void stopSimulation();
		Q_INVOKABLE void saveSimulation(const QString& url);
		Q_INVOKABLE void loadSimulation(const QString& url);
		Q_INVOKABLE void userInput(char input);
		Q_INVOKABLE void addBlock(int x, int y, int z);
		Q_INVOKABLE void destroyBlock(int x, int y, int z);

		Q_INVOKABLE void testing(const QString& value);
		Q_INVOKABLE void playtest(const QString& url);
		Q_INVOKABLE void stoptest(const QString& url);
		Q_INVOKABLE void updateTestSlider(const float azimuth, const float elevation);


	signals:
		void avatarPosChanged();
		void headAnglesChanged();

	private:
		ef::Vector3F m_pos;
		ef::Vector3F m_angles;
		EFView* m_parent;
};
#pragma once
#include <QObject>
#include <string>
#include "Vector.h"

class EFView;

#ifndef UICOM_H
	#define UICOM_H
	
	/// <summary>
	/// Testing
	/// https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html
	/// </summary>
	class UICommunicator: public QObject {
		Q_OBJECT
		public:
			UICommunicator(EFView* view);
			UICommunicator(EFView* view, ef::Vector3 pos, ef::Vector3 angles);
			// READ -> Getter WRITE -> Setter NOTIFY -> SIGNAL
			Q_PROPERTY(ef::Vector3 avatarPos READ avatarPos WRITE setAvatarPos NOTIFY avatarPosChanged);
			Q_PROPERTY(ef::Vector3 headAngles READ headAngles WRITE setHeadAngles NOTIFY headAnglesChanged);


			const ef::Vector3& avatarPos() const;
			const ef::Vector3& headAngles() const;

			void setAvatarPos(ef::Vector3& v);
			void setHeadAngles(ef::Vector3& v);

			Q_INVOKABLE void startSimulation();
			Q_INVOKABLE void stopSimulation();
			Q_INVOKABLE void saveSimulation(std::string url);
			Q_INVOKABLE void loadSimulation(std::string url);
			Q_INVOKABLE void userInput(char input);
			Q_INVOKABLE void addBlock(int x, int y, int z);
			Q_INVOKABLE void destroyBlock(int x, int y, int z);

		signals:
			void avatarPosChanged();
			void headAnglesChanged();

		private:
			ef::Vector3 m_pos;
			ef::Vector3 m_angles;
			EFView* m_parent;

	};

#endif
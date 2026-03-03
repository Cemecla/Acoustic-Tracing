#include "EFController.h"
#include <QGuiApplication>
#include <qicon.h>
#include "Avatar.h"


int main(int argc, char *argv[])
{
    #if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif
    
    // Creation of the QGuiApplication
    // This object must reside inside the main
    QGuiApplication app(argc, argv);

    // Set the window logo 
    app.setWindowIcon(QIcon(":/images/logo"));

    // Creation of the Echo Forge Controller
    EFController* efc = new EFController();

    // Execution of QT Application
    return app.exec();
}

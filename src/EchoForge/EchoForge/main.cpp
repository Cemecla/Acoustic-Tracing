#include "EFController.h"
#include <QQuickStyle>
#include <QGuiApplication>
#include <qicon.h>
#include "Avatar.h"
#include "JuceHeader.h"
#include "SpatialAudioEngine.h"




int main(int argc, char *argv[])
{
    #if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    // Creation of the QGuiApplication
    // This object must reside inside the main
    QGuiApplication app(argc, argv);
    
    // This perfectly bridges JUCE's background requirements with Qt's main thread.
    juce::ScopedJuceInitialiser_GUI juceInit;

    // Set the window logo 
    app.setWindowIcon(QIcon(":/images/logo"));
    QQuickStyle::setStyle("Basic");


    YutaSound::SpatialAudioEngine engine;
    engine.prepare(48000.0,512);
    engine.addSource("../../../../Acoustic-Tracing/assets/mooo1945.mp3");
    // engine.start();


    // Creation of the Echo Forge Controller
    EFController* efc = new EFController();

    // Execution of QT Application
    return app.exec();
}

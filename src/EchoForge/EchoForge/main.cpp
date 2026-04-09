#include <QGuiApplication>

#include "EFController.h"
#include "EFView.h"

#include "Avatar.h"

#include "JuceHeader.h"
#include "AudioManager.h"
#include "SpatialAudioEngine.h"

using namespace yutasound;
using namespace juce;

int main(int argc, char *argv[])
{
    #if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    // Creation of the QGuiApplication
    // This object must reside inside the main
    QGuiApplication app(argc, argv);
    
    // This perfectly bridges JUCE's background requirements with Qt's main thread.
    ScopedJuceInitialiser_GUI juceInit;

    AudioManager audioManager;
    SpatialAudioEngine engine(audioManager);

    audioManager.prepare(48000.0, 512);
    engine.prepare(48000.0, 512);
    engine.addSource("../../../../Acoustic-Tracing/assets/mooo.wav");
    engine.start();

    // Creation of the Echo Forge Controller
    EFController* efc = new EFController();

    efc->m_view->setupQGuiApplication(app);

    efc->m_view->setupAudioManager(&audioManager);

    //efc->m_view->setupAudioEngine(&engine);

    // Execution of QT Application
    return app.exec();
}

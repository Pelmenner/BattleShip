#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QTime>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include <QFileInfo>

#ifdef Q_OS_ANDROID
    #include <QtAndroid>
#endif // Q_OS_ANDROID

#include "game.h"
#include "field.h"
#include "player.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    int id = QFontDatabase::addApplicationFont(":/data/Quando-Regular.ttf");
    if (QFontDatabase::applicationFontFamilies(id).empty())
    {
        qDebug("Could not load font Quando-Regular (cpp)");
        return id;
    }
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont mainFont(fontFamily);
    QGuiApplication::setFont(mainFont);

    qmlRegisterType<Cell>("backend.Cell", 1, 0, "Cell");
    qmlRegisterType<Field>("backend.Field", 1, 0, "BackendField");
    qmlRegisterType<Player>("backend.Player", 1, 0, "BackendPlayer");

#ifdef Q_OS_ANDROID
    // set statusbar color
    QtAndroid::runOnAndroidThread([]()
    {
        QAndroidJniObject window = QtAndroid::androidActivity().callObjectMethod("getWindow", "()Landroid/view/Window;");
        window.callMethod<void>("addFlags", "(I)V", 0x80000000);
        window.callMethod<void>("clearFlags", "(I)V", 0x04000000);
        window.callMethod<void>("setStatusBarColor", "(I)V", 0xff252525);
        QtAndroid::hideSplashScreen(800);
    });
#endif // Q_OS_ANDROID

    Game game;

    return app.exec();
}

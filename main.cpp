#include "dialog.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    :/imgs/rc/images/splash.jpg
    /**
     * @brief 启动界面图像
     */
    QPixmap pixmap(":/imgs/rc/images/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    /**
     * @brief loop
     * loading the splash
     */
    QEventLoop loop;
    QTimer::singleShot(2000, &loop, SLOT(quit()));
    loop.exec();

    Dialog w;
    w.show();
    splash.finish(&w );
    return a.exec();
}

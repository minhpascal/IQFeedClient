#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

#include "iqFeedClient.h"

int main ( int argc, char* argv[] ) {
    QCoreApplication app ( argc, argv );
    IQFeedClient IQFeedClient;

    if ( argc < 2 ) {
        qDebug() << "Usage: IQFeedClient.exe <config.ini>";
        return 0;
    }

    QString settingsFile;
    settingsFile.append ( argv[1] );
    IQFeedClient.init ( settingsFile );

    return app.exec();
}

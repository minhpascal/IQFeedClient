#include "iqFeedClient.h"

IQFeedClient::IQFeedClient ( QObject* parent ) :
    QObject ( parent ) {
}

IQFeedClient::~IQFeedClient() {
}

void IQFeedClient::init ( const QString& settingsFile ) {
    QSettings* settings = new QSettings ( settingsFile, QSettings::IniFormat );

    settings->beginGroup ( "IQFeed" );
    QString IQConnectPath = settings->value ( "root" ).toString();
    QString connectionInfo = settings->value ( "connectionInfo" ).toString();
    settings->endGroup();

    qDebug() << IQConnectPath;
    qDebug() << connectionInfo;

    m_processManager = std::shared_ptr<ProcessManager> ( new ProcessManager() );
    connect ( m_processManager.get(), SIGNAL ( processStartupCompleted() ), this, SLOT ( setupSocketConnections() ) );
    startConnectionManager ( IQConnectPath, connectionInfo );
}

void IQFeedClient::startConnectionManager ( const QString& IQFeedPath, const QString& connectionInfo ) {
    m_processManager->setup ( IQFeedPath, connectionInfo );
}

void IQFeedClient::setupSocketConnections() {
    qDebug() << __FUNCTION__ << " - Setting up socket connections";
    QThread::sleep ( 3 );

    m_iqConnectConnection = std::shared_ptr<IQConnectConnection> ( new IQConnectConnection ( this ) );
}

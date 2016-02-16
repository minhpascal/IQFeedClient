#include <cstdio>
#include <QDebug>
#include <QStringList>
#include <QAbstractSocket>

#include "processManager.h"

const QString protocolVersion ( "5.0" );

ProcessManager::ProcessManager ( QObject* parent ) :
    QObject ( parent ) {
    qRegisterMetaType< QAbstractSocket::SocketError >();
}

ProcessManager::~ProcessManager() {
    m_DTNIQProcess->close();
    delete m_DTNIQProcess;
}

void ProcessManager::setup ( const QString& IQFeedPath, const QString& connectionInfo ) {
    launchClient ( IQFeedPath, connectionInfo );
}

void ProcessManager::launchClient ( const QString& IQFeedPath, const QString& connectionInfo ) {
    QStringList program_arguments;
    program_arguments = QStringList() << connectionInfo;

    // Create process and hook into signals
    m_DTNIQProcess = new QProcess ( this );
    connect ( m_DTNIQProcess, SIGNAL ( started() ), this, SLOT ( processStarted() ) );
    connect ( m_DTNIQProcess, SIGNAL ( readyReadStandardOutput() ), this, SLOT ( processStdOut() ) );
    connect ( m_DTNIQProcess, SIGNAL ( readyReadStandardError() ), this, SLOT ( processStdErr() ) );
    connect ( m_DTNIQProcess, SIGNAL ( finished ( int ) ), this, SLOT ( processFinished ( int ) ) );

    // Start the DTN IQConnect process
    qDebug() << "Starting iqconnect...";
    if ( !IQFeedPath.isEmpty() ) {
        m_DTNIQProcess->setWorkingDirectory ( IQFeedPath );
    }
    QString m_program = "iqconnect.exe";

    m_DTNIQProcess->start ( m_program, program_arguments );
}

void ProcessManager::processStarted() {
    qDebug() << "The process has started";
    emit processStartupCompleted();
}

void ProcessManager::processFinished ( int result ) {
    qDebug() << "The process has finished with result " << result;
    emit restartProcessManager();
}

void ProcessManager::processStdOut() {
    QByteArray msg = m_DTNIQProcess->readAllStandardOutput();
    qDebug() << QString::fromLocal8Bit ( msg );
}

void ProcessManager::processStdErr() {
    QByteArray msg = m_DTNIQProcess->readAllStandardError();
    qDebug() << QString::fromLocal8Bit ( msg );
}

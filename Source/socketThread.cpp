#include <cstdio>
#include <QtNetwork>

#include "socketThread.h"

SocketThread::SocketThread ( QObject* parent ) :
    QThread ( parent ),
    m_quit ( false ) {
}

SocketThread::~SocketThread() {
    m_quit = true;
    wait();
}

void SocketThread::initConnection ( const QString& host_name, quint16 port ) {
    qDebug() << __FUNCTION__ << " - host = " << host_name << ":" << port;
    this->m_hostName = host_name;
    this->m_port = port;
    start();
}

void SocketThread::run() {
    const int Timeout = 5 * 1000;

    m_socket.connectToHost ( m_hostName, m_port );
    if ( !m_socket.waitForConnected ( Timeout ) ) {
        emit socketError ( m_socket.error(), m_socket.errorString() );
        return;
    }

    while ( !m_quit ) {
        if ( !m_socket.waitForReadyRead ( Timeout ) ) {
            emit socketError ( m_socket.error(), m_socket.errorString() );
            return;
        }
        QByteArray data = m_socket.readAll();
        emit socketData ( data );
    }
}

void SocketThread::send ( QString str ) {
    m_socket.write ( str.toStdString().c_str() );
}

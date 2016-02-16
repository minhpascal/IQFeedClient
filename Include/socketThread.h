#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QAbstractSocket>
#include <QtNetwork>

class SocketThread : public QThread {
    Q_OBJECT

  public:
    SocketThread ( QObject* parent = 0 );
    ~SocketThread();

    void initConnection ( const QString& host_name, quint16 port );
    void run();
    void send ( QString str );

  signals:
    void socketData ( const QByteArray& data );
    void socketError ( QAbstractSocket::SocketError socket_error, const QString& message );

  private:
    QTcpSocket m_socket;
    QString m_hostName;
    quint16 m_port;
    bool m_quit;
};

#endif // SOCKETTHREAD_H

#ifndef IQCLIENTCONNECTION_H
#define IQCLIENTCONNECTION_H

#include <list>
#include "connection.h"

class IQFeedClient;

class IQConnectConnection : public Connection {
    Q_OBJECT

  public:
    IQConnectConnection ( IQFeedClient* iqFeedClient );
    ~IQConnectConnection();

  private:
    void processSystemMessage ( const QString& message );
    void processQuote ( const QString& data );
    void processMessage ( const QString& data );

    IQFeedClient* m_iqFeedClient;

  signals:
    void serverConnected();

  private slots:
    void processSocketData ( QByteArray data );
    void processSocketError ( QAbstractSocket::SocketError socket_error, const QString& message );
};

#endif // IQCLIENTCONNECTION_H

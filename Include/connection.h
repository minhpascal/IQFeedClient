#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>

#include "socketThread.h"

class Connection : public QObject {
    Q_OBJECT

  public:
    Connection ( QObject* parent = 0 );
    ~Connection();

  protected:
    SocketThread m_connectionThread;
    QString m_symbol;
    qint32 m_messageCount;

    bool messageContains ( const QString& message, const QString& token );
};

#endif // CONNECTION_H

#ifndef IQFEEDCLIENT_H
#define IQFEEDCLIENT_H

#include <memory>
#include <fstream>
#include <QTime>
#include <QString>

#include "processManager.h"
#include "iqConnectConnection.h"

class IQFeedClient : public QObject {

    Q_OBJECT

  public:
    IQFeedClient ( QObject* parent = 0 );
    ~IQFeedClient();

    void init ( const QString& settingsFile );

  private:
    std::shared_ptr<ProcessManager> m_processManager;
    std::shared_ptr<IQConnectConnection> m_iqConnectConnection;

  private slots:
    void startConnectionManager ( const QString& IQFeedPath, const QString& connectionInfo );
    void setupSocketConnections();
};

#endif // IQFEEDCLIENT_H

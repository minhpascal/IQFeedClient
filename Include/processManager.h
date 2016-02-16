#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QProcess>

class ProcessManager : public QObject {
    Q_OBJECT

  public:
    ProcessManager ( QObject* parent = 0 );
    ~ProcessManager();

    void setup ( const QString& IQFeedPath, const QString& connectionInfo );

  private:
    QProcess* m_DTNIQProcess;
    QString m_symbol;

    void launchClient ( const QString& IQFeedPath, const QString& connectionInfo );

  signals:
    void restartProcessManager();
    void processStartupCompleted();

  private slots:
    void processStarted();
    void processFinished ( int result );
    void processStdOut();
    void processStdErr();
};

#endif // PROCESSMANAGER_H

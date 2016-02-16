#ifndef PROTECTEDBUFFER_H
#define PROTECTEDBUFFER_H

#include <QByteArray>
#include <QMutex>

/*
    Simple wrapper class that adds mutex lock access support
    so that we can protect access to shared QByteArray
*/

class ProtectedBuffer {
  public:
    ProtectedBuffer ();

    qint32 size();
    QByteArray left ( qint32 len );
    qint32 append ( const QByteArray& ba );

  private:
    QMutex mRWLock;
    QByteArray mBuffer;
};

#endif // PROTECTEDBUFFER_H

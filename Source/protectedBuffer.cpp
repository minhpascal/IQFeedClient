#include "protectedBuffer.h"

ProtectedBuffer::ProtectedBuffer() {
}

// Return the buffer size in bytes
qint32 ProtectedBuffer::size() {
    mRWLock.lock();
    qint32 size = mBuffer.size();
    mRWLock.unlock();
    return size;
}

// Return left most (len) elements of buffer
QByteArray ProtectedBuffer::left ( qint32 len ) {
    mRWLock.lock();
    QByteArray ba = mBuffer.left ( len );
    mBuffer.remove ( 0, len );
    mRWLock.unlock();
    return ba;
}

// Appnd the byte array (ba) to end of buffer
qint32 ProtectedBuffer::append ( const QByteArray& ba ) {
    mRWLock.lock();
    mBuffer.append ( ba );
    qint32 size = mBuffer.size();
    mRWLock.unlock();
    return size;
}

#include "connection.h"

Connection::Connection ( QObject* parent )
    : QObject ( parent ),
      m_messageCount ( 0 ) {
}

Connection::~Connection() {
}

bool Connection::messageContains ( const QString& message, const QString& token ) {
    if ( message.contains ( token ) ) {
        return true;
    }
    return false;
}

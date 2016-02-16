#include "iqConnectConnection.h"
#include "iqFeedClient.h"

const QString protocolVersion ( "5.0" );
const quint16 IQConnectPort = 5009;

IQConnectConnection::IQConnectConnection ( IQFeedClient* iqFeedClient ) : m_iqFeedClient ( iqFeedClient ) {
    connect ( &m_connectionThread, SIGNAL ( socketData ( QByteArray ) ), this,
              SLOT ( processSocketData ( QByteArray ) ) );
    connect ( &m_connectionThread, SIGNAL ( socketError ( QAbstractSocket::SocketError, QString ) ), this,
              SLOT ( processSocketError ( QAbstractSocket::SocketError, QString ) ) );

    qDebug() << __FUNCTION__ << " - Calling m_IQConnectThread.initConnection";
    m_connectionThread.initConnection ( "127.0.0.1", IQConnectPort );
}

IQConnectConnection::~IQConnectConnection() {
}

void IQConnectConnection::processSocketData ( QByteArray data ) {
    QByteArray m_receiveBuffer;
    QString message;

    if ( data.length() > 0 ) {
        m_receiveBuffer += data;
        qint32 endMarkerIndex;
        while ( ( endMarkerIndex = m_receiveBuffer.indexOf ( "\n" ) ) >= 0 ) {
            message = m_receiveBuffer.left ( endMarkerIndex );
            char commandID = m_receiveBuffer[0];
            message.remove ( 0, 2 );
            switch ( commandID ) {
            case 'S':
                processSystemMessage ( message );
                break;
            case 'Q':
                processQuote ( message );
                break;
            default:
                processMessage ( message );
                break;
            }
            m_receiveBuffer = m_receiveBuffer.mid ( endMarkerIndex + 1 );
        }
    }
}

void IQConnectConnection::processSystemMessage ( const QString& message ) {
    QString token;
    token.sprintf ( "SERVER CONNECTED" );
    if ( messageContains ( message, token ) ) {
        qDebug() << __FUNCTION__ << " : " << message;
        QString command;
        command.sprintf ( "S,SET PROTOCOL,%s\r\n", protocolVersion.toStdString().c_str() );
        m_connectionThread.send ( command.toStdString().c_str() );
        emit serverConnected();
    }
    token.sprintf ( "CURRENT PROTOCOL,%s,", protocolVersion.toStdString().c_str() );
    if ( messageContains ( message, token ) ) {
        qDebug() << __FUNCTION__ << " : " << message;

        // After successful connection we send request for symbol(s) and fields we require
        QString command;
        command.sprintf ( "S,SELECT UPDATE FIELDS,Last TimeMS,Last,Last Size,Bid,Bid Size,Ask,Ask Size,Tick\r\n" );
        m_connectionThread.send ( command );

        // E-Mini S&P 500
        m_symbol = "@ES#";
        qDebug() << "Requesting update for symbol " << m_symbol;
        command.sprintf ( "w%s\r\n", m_symbol.toStdString().c_str() );
        m_connectionThread.send ( command );

        // E-Mini Nasdaq 100
        m_symbol = "@NQ#";
        qDebug() << "Requesting update for symbol " << m_symbol;
        command.sprintf ( "w%s\r\n", m_symbol.toStdString().c_str() );
        m_connectionThread.send ( command );

        // E-Mini Dow
        m_symbol = "@YM#";
        qDebug() << "Requesting update for symbol " << m_symbol;
        command.sprintf ( "w%s\r\n", m_symbol.toStdString().c_str() );
        m_connectionThread.send ( command );
    }
}

// Results are returned in the format: @ES#,03:49:18.236,1871.25,12,1871.00,120,1871.25,177, 0
void IQConnectConnection::processQuote ( const QString& data ) {
    QStringList results = data.split ( "," );
    QString symbol = results[0];

    float last = results[2].toFloat();
    quint32 lastSize = results[3].toUInt();
    float bid = results[4].toFloat();
    quint32 bidSize = results[5].toUInt();
    float ask = results[6].toFloat();
    quint32 askSize = results[7].toUInt();

    qDebug() << "symbol = " << symbol << ", last = " << last << ", lastSize = " << lastSize << ", bid = " << bid <<
             ", ask = " << ask;
}

void IQConnectConnection::processMessage ( const QString& data ) {
    QDateTime quoteTime = QDateTime::fromString ( data, "yyyyMMdd hh:mm:ss" );
    std::string quoteTimeStr = quoteTime.toString().toStdString();
    if ( quoteTimeStr.length() == 0 ) {
        // Skip invalid time stamp
        return;
    }

    // Convert quote time to Chicago time (CME)
    quoteTime.setTimeSpec ( Qt::OffsetFromUTC );
    quoteTime.setOffsetFromUtc ( - ( 9 * 3600 ) );
    quoteTime = quoteTime.toLocalTime();

    qDebug() << quoteTime.toString ( Qt::ISODate );
}

void IQConnectConnection::processSocketError ( QAbstractSocket::SocketError socket_error,
        const QString& message ) {
    qDebug() << __FUNCTION__ << message;
    switch ( socket_error ) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ConnectionRefusedError";
        qDebug() << "Waiting to reconnect... ";
        QThread::sleep ( 3 );
        qDebug() << "Trying";
        m_connectionThread.initConnection ( "127.0.0.1", IQConnectPort );
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "SocketTimeoutError";
        qDebug() << "Waiting to reconnect... ";
        QThread::sleep ( 3 );
        qDebug() << "Trying";
        m_connectionThread.initConnection ( "127.0.0.1", IQConnectPort );
        break;

    case QAbstractSocket::DatagramTooLargeError:
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::HostNotFoundError:
    case QAbstractSocket::SocketAccessError:
    case QAbstractSocket::SocketResourceError:
    case QAbstractSocket::NetworkError:
    case QAbstractSocket::AddressInUseError:
    case QAbstractSocket::SocketAddressNotAvailableError:
    case QAbstractSocket::UnsupportedSocketOperationError:
    case QAbstractSocket::UnfinishedSocketOperationError:
    case QAbstractSocket::ProxyAuthenticationRequiredError:
    case QAbstractSocket::SslHandshakeFailedError:
    case QAbstractSocket::ProxyConnectionRefusedError:
    case QAbstractSocket::ProxyConnectionClosedError:
    case QAbstractSocket::ProxyConnectionTimeoutError:
    case QAbstractSocket::ProxyNotFoundError:
    case QAbstractSocket::ProxyProtocolError:
    case QAbstractSocket::OperationError:
    case QAbstractSocket::SslInternalError:
    case QAbstractSocket::SslInvalidUserDataError:
    case QAbstractSocket::TemporaryError:
    case QAbstractSocket::UnknownSocketError:
        break;
    }
}

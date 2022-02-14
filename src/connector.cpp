#include "connector.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

Connector::Connector(QObject *parent)
    : QObject(parent), m_clientSocket(new QTcpSocket(this)), m_loggedIn(false)
{
    connect(m_clientSocket, &QTcpSocket::connected, this, &Connector::connected);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Connector::disconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Connector::onReadyRead);
    connect(m_clientSocket, &QAbstractSocket::errorOccurred, this, &Connector::error);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]() -> void { m_loggedIn = false; });
}

void Connector::lookForOpponent()
{
    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("connect"))}));
}

void Connector::sendField(const QString &fieldStr)
{
    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("field")),
                       qMakePair(QStringLiteral("field"), QJsonValue(fieldStr))}));
}

void Connector::sendMove(const int x, const int y)
{
    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("move")),
                       qMakePair(QStringLiteral("x"), QJsonValue(x)),
                       qMakePair(QStringLiteral("y"), QJsonValue(y))}));
}

void Connector::createRoom()
{
    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("create"))}));
}

void Connector::joinRoom(int roomId)
{
    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("join")),
                       qMakePair(QStringLiteral("roomId"), QJsonValue(roomId))}));
}

void Connector::sendMessage(const QList<QPair<QString, QJsonValue>> &values)
{
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);
    QJsonObject message;
    for (const auto& [key, value]: values)
        message[key] = value;
    clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void Connector::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}

void Connector::connectToServer(const QHostAddress &address, quint16 port, const QString &playerName)
{
    m_clientSocket->connectToHost(address, port);
    if (m_clientSocket->waitForConnected())
        qDebug("Connected");
    else
        qDebug("Could not connect to server");

    sendMessage(QList({qMakePair(QStringLiteral("type"), QJsonValue("name")),
                       qMakePair(QStringLiteral("name"), QJsonValue(playerName))}));
}

void Connector::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;

    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0)
    { //It's a login message
        if (m_loggedIn)
            return;

        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return;
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess)
        {
            emit loggedIn();
            return;
        }

        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    }
    else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
        if (textVal.isNull() || !textVal.isString())
            return;
        if (senderVal.isNull() || !senderVal.isString())
            return;
        emit messageReceived(senderVal.toString(), textVal.toString());
    }
    else if (typeVal == "gameStarted")
    {
        emit gameStarted();
    }
    else if (typeVal == "opponentFound")
    {
        emit opponentFound();
    }
    else if (typeVal == "turn")
    {
        QJsonValue turn = docObj.value(QStringLiteral("turn"));
        emit gotTurn(turn.toInt());
    }
    else if (typeVal == "field")
    {
        QJsonValue cells = docObj.value(QStringLiteral("field"));
        QJsonValue player = docObj.value(QStringLiteral("player"));
        emit gotField(cells.toString(), player.toInt());
    }
    else if (typeVal == "gameEnded")
    {
        qDebug("Game ended");
        QJsonValue isWinner = docObj.value(QStringLiteral("isWinner"));
        emit gameFinished(isWinner.toBool());
    }
    else if (typeVal == "roomCreated")
    {
        QJsonValue roomId = docObj.value(QStringLiteral("roomId"));
        emit roomIdRecieved(roomId.toInt());
    }
}

void Connector::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);
    for (;;)
    {
        // we start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction())
        {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError)
            {
                if (jsonDoc.isObject())
                    jsonReceived(jsonDoc.object());
            }
        }
        else
        { // the read failed, the socket goes automatically back to the state it was in before the transaction started
            break;
        }
    }
}

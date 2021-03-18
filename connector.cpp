#include "connector.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

Connector::Connector(QObject* parent)
	: QObject(parent)
	, m_clientSocket(new QTcpSocket(this))
	, m_loggedIn(false)
{
	connect(m_clientSocket, &QTcpSocket::connected, this, &Connector::connected);
	connect(m_clientSocket, &QTcpSocket::disconnected, this, &Connector::disconnected);
	// connect readyRead() to the slot that will take care of reading the data in
	connect(m_clientSocket, &QTcpSocket::readyRead, this, &Connector::onReadyRead);
    // Forward the error signal, QOverload is necessary as error() is overloaded
	connect(m_clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connector::error);
	// Reset the m_loggedIn variable when we disconnect. Since the operation is trivial we use a lambda instead of creating another slot
	connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]()->void {m_loggedIn = false; });
}

void Connector::lookForOpponent()
{
	QDataStream clientStream(m_clientSocket);
	clientStream.setVersion(QDataStream::Qt_5_7);
	QJsonObject message;
	message["type"] = QStringLiteral("connect");
	clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void Connector::sendField(const QString& fieldStr)
{
	QDataStream clientStream(m_clientSocket);
	clientStream.setVersion(QDataStream::Qt_5_7);
	QJsonObject message;
	message["type"] = QStringLiteral("field");
	message["arg"] = fieldStr;
	clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void Connector::sendMove(const int x, const int y)
{
	QDataStream clientStream(m_clientSocket);
	clientStream.setVersion(QDataStream::Qt_5_7);
	QJsonObject message;
	message["type"] = QStringLiteral("field");
	message["x"] = x;
	message["y"] = y;
	clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void Connector::login(const QString& userName)
{
    if (m_clientSocket->state() == QAbstractSocket::ConnectedState)
    {   // if the client is connected
		// create a QDataStream operating on the socket
		QDataStream clientStream(m_clientSocket);
		// set the version so that programs compiled with different versions of Qt can agree on how to serialise
		clientStream.setVersion(QDataStream::Qt_5_7);
		// Create the JSON we want to send
		QJsonObject message;
		message["type"] = QStringLiteral("login");
		message["username"] = userName;
		// send the JSON using QDataStream
		clientStream << QJsonDocument(message).toJson();
	}
}

void Connector::sendMessage(const QString& text)
{
	if (text.isEmpty())
		return;
	QDataStream clientStream(m_clientSocket);
	// set the version so that programs compiled with different versions of Qt can agree on how to serialise
	clientStream.setVersion(QDataStream::Qt_5_7);
	// Create the JSON we want to send
	QJsonObject message;
	message["type"] = QStringLiteral("message");
	message["text"] = text;
	// send the JSON using QDataStream
	clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
}

void Connector::disconnectFromHost()
{
	m_clientSocket->disconnectFromHost();
}
void Connector::connectToServer(const QHostAddress& address, quint16 port)
{
	m_clientSocket->connectToHost(address, port);
	if (m_clientSocket->waitForConnected()) // putting 1 as parameter isn't reasonable, using default 3000ms value
		qDebug("Connected");
	else
		qDebug("Could not connect to server");
}

void Connector::jsonReceived(const QJsonObject& docObj)
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
		// the login attempt failed, we extract the reason of the failure from the JSON
		// and notify it via the loginError signal
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
}

void Connector::onReadyRead()
{
	// prepare a container to hold the UTF-8 encoded JSON we receive from the socket
	QByteArray jsonData;
	// create a QDataStream operating on the socket
	QDataStream socketStream(m_clientSocket);
	// set the version so that programs compiled with different versions of Qt can agree on how to serialise
	socketStream.setVersion(QDataStream::Qt_5_7);
    for (;;)
    {
		// we start a transaction so we can revert to the previous state in case we try to read more data than is available on the socket
		socketStream.startTransaction();
		// we try to read the JSON data
		socketStream >> jsonData;
        if (socketStream.commitTransaction())
        {   // we successfully read some data
			// we now need to make sure it's in fact a valid JSON
			QJsonParseError parseError;
			// we try to create a json document with the data we received
			const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError)
            { // if the data was indeed valid JSON
				if (jsonDoc.isObject()) // and is a JSON object
					jsonReceived(jsonDoc.object()); // parse the JSON
			}
		}
		else {
			// the read failed, the socket goes automatically back to the state it was in before the transaction started
			// we just exit the loop and wait for more data to become available
			break;
		}
	}
}

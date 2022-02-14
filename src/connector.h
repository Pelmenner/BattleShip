#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QTcpSocket>

class Connector : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Connector)

public:
    explicit Connector(QObject *parent = nullptr);
    void lookForOpponent();
    void sendField(const QString &fieldStr);
    void sendMove(const int x, const int y);
    void createRoom();
    void joinRoom(int roomId);
    void sendMessage(const QList<QPair<QString, QJsonValue>>& values);

public slots:
    void connectToServer(const QHostAddress &address, quint16 port, const QString &playerName);
    void disconnectFromHost();

private slots:
    void onReadyRead();

signals:
    void connected();
    void loggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
    void gameStarted();
    void opponentFound();
    void gotTurn(int isCurrent);
    void gotField(const QString &cellString, int player);
    void gameFinished(bool isWinner);
    void roomIdRecieved(int roomId);

private:
    QTcpSocket *m_clientSocket;
    bool m_loggedIn;
    void jsonReceived(const QJsonObject &doc);
};

#endif // CONNECTOR_H

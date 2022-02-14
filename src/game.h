#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "field.h"

class Connector;

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Field* curField READ getCurField NOTIFY curFieldChanged)
    Q_PROPERTY(Field* firstField READ getFirstField NOTIFY firstFieldChanged)
    Q_PROPERTY(Field* secondField READ getSecondField NOTIFY secondFieldChanged)
    Q_PROPERTY(QString waitingInfo READ getWaitingInfo NOTIFY waitingInfoChanged)

public:
    explicit Game(QObject *parent = nullptr);

    Field *getCurField();
    Field *getFirstField();
    Field *getSecondField();
    QString getWaitingInfo();

    void setNames(const QString &name1, const QString &name2);

    QPair<Field *, Field *> GetFields();

signals:
    void curFieldChanged();
    void firstFieldChanged();
    void secondFieldChanged();

    void localGameStarted();
    void initializationStarted();
    void nameInputStarted();
    void gameClosed();
    void returnedHome();
    void opponentSelectionStarted();
    void waiting();
    void waitingInfoChanged();

public slots:
    void startInitialization();
    void setTurn(int isCurrent);

    void okClicked();
    void clearClicked();
    void randomClicked();
    void exit();
    void playClicked(bool online);
    void restartGame(bool saveNames);

    void namesInputed(const QString& name1, const QString& name2);
    void gameFinished();

    void fieldInitRClicked(int x, int y);
    void fieldInitClicked(int x, int y);
    void fieldPlayClicked(int x, int y);

    void randomOpponentClicked(const QString& serverAddress, const QString& serverPort, const QString& playerName);
    void createRoomClicked(const QString& serverAddress, const QString& serverPort, const QString& playerName);
    void joinRoomClicked(const QString& serverAddress, const QString& serverPort, const QString& playerName, const QString& roomId);

    Q_INVOKABLE void returnHome();
private:
    enum class GameState {begin, initPlayer1, initPlayer2, play, connect};
    GameState gameState = GameState::begin;

    bool isOnline = false;
    bool drawing = false;

    QQmlApplicationEngine engine;
    QString waitingInfo;
    Field *field1, *field2, *curField;
    Connector *connector;
    QPoint bufPos;
    int turn;

    void initPlayer(Field *field);
    void finishPlayerInit();

    void playLocal();
    void endLocalGame();
    void startOpponentSelection();
    void connectToServer(const QString& serverAddress, const QString& serverPort, const QString& playerName);

    void switchField();

private slots:
    void playOnline();

    void messageReceived(const QString &sender, const QString &text);
    void connectionError(const QString& error);

    void fieldRecieved(const QString& cellString, int player);
    void onlineGameFinished(bool isWinner);
    void changeWaitingInfo(const QString& info);
};

#endif // GAME_H

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

public:
    explicit Game(QObject *parent = nullptr);

    Field *getCurField();
    Field *getFirstField();
    Field *getSecondField();

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

public slots:
    void startInitialization();
    void setTurn(int isCurrent);

private:
    enum class GameState {begin, initPlayer1, initPlayer2, play, connect};
    GameState gameState = GameState::begin;

    bool isOnline = false;
    bool drawing = false;

    QQmlApplicationEngine engine;
    QQmlContext *context;
    QObject* initializer;
    QObject* frontField1, *frontField2;
    QObject* homePage;
    QObject* mainWindow;
    QObject* namesInput;
    QObject* frontGame;
    QObject* restartPage;
    QObject* opponentSelectPage;

    Field *field1, *field2, *curField;
    Connector *connector;
    QPoint bufPos;
    int turn;

    void initPlayer(Field *field);
    void finishPlayerInit();

    void playLocal();
    void endLocalGame();
    void startOpponentSelection();
    void connectToServer(const QString& serverAddress, const QString& serverPort);

private slots:
    void playOnline();
    void fieldInitRClicked(int x, int y);
    void fieldInitClicked(int x, int y);
    void fieldPlayClicked(int x, int y);

    void okClicked();
    void clearClicked();
    void randomClicked();
    void exitClicked();
    void homeClicked();
    void playClicked(bool online);
    void gameRestarted(bool saveNames);

    void homePageLoaded();
    void nameInputLoaded();
    void initLoaded();
    void gameLoaded();

    void namesInputed();
    void gameFinished();
    void restartPageLoaded();
    void opponentSelectPageLoaded();
    void waitingPageLoaded();

    void randomOpponentClicked(const QString& serverAddress, const QString& serverPort);
    void friendOpponentClicked(const QString& serverAddress, const QString& serverPort);

    void messageReceived(const QString &sender, const QString &text);
    void connectionError(const QString& error);

    void fieldRecieved(const QString& cellString, int player);
    void onlineGameFinished(bool isWinner);
};

#endif // GAME_H

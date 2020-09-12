#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "field.h"

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
    void initPlayer(Field *field);

    void playLocal();
    void endLocalGame();

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

public slots:
    void startGame();

private:
    enum class GameMode {local, online};
    enum class GameState {begin, initPlayer1, initPlayer2, play, connect};
    GameMode gameMode = GameMode::local;
    GameState gameState = GameState::begin;

    QQmlApplicationEngine engine;
    QQmlContext *context;
    QObject* initializer;
    QObject* frontField1, *frontField2;
    QObject* homePage;
    QObject* mainWindow;
    QObject* namesInput;
    QObject* frontGame;
    QObject* restartPage;

    Field *field1, *field2;
    QRect *hider;

    bool drawing = false;
    Field *curField;
    QPoint bufPos;

    void finishPlayerInit();

private slots:
    void fieldInitRClicked(int x, int y);
    void fieldInitClicked(int x, int y);
    void fieldPlayClicked(int x, int y);
    void randomClicked();
    void clearClicked();
    void okClicked();
    void playClicked();
    void homePageLoaded();
    void initLoaded();
    void gameLoaded();
    void nameInputLoaded();
    void namesInputed();
    void gameRestarted(bool saveNames);
    void gameFinished();
    void restartPageLoaded();
    void exitClicked();
    void homeClicked();
};

#endif // GAME_H

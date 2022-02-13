#include "game.h"
#include "connector.h"
#include <QGuiApplication>
#include <QHostAddress>
#include <QQmlContext>
#include <QDebug>
#include <QTimer>

Game::Game(QObject *parent)
    : QObject(parent),
      field1(new Field(this)),
      field2(new Field(this)),
      curField(field1),
      connector(new Connector(this)), turn(0)
{
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("backend", this);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
}

Field *Game::getCurField()
{
    return curField;
}

Field *Game::getFirstField()
{
    return field1;
}

Field *Game::getSecondField()
{
    return field2;
}

QString Game::getWaitingInfo()
{
    return waitingInfo;
}

void Game::finishPlayerInit()
{
    drawing = false;
    curField->clearCells();
    curField = 0;
}

void Game::setNames(const QString &name1, const QString &name2)
{
    field1->setName(name1);
    field2->setName(name2);
}

void Game::initPlayer(Field *field)
{
    curField = field;
    emit curFieldChanged();

    if (field == field1)
        gameState = GameState::initPlayer1;
    else
        gameState = GameState::initPlayer2;

    drawing = false;
}

void Game::playLocal()
{
    emit localGameStarted();
    connect(field1, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int, int)));
    connect(field2, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int, int)));

    curField = field1;
    emit curFieldChanged();
}

void Game::playOnline()
{
    emit localGameStarted();
}

void Game::endLocalGame()
{
    disconnect(field1, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int, int)));
    disconnect(field2, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int, int)));

    if (curField == field1)
        field2->showAlive();
    else
        field1->showAlive();

    curField = nullptr;
    emit curFieldChanged();
}

void Game::startOpponentSelection()
{
    emit opponentSelectionStarted();
}

void Game::connectToServer(const QString &serverAddress, const QString &serverPort, const QString &playerName)
{
    qDebug("Connecting to server...");
    connect(connector, &Connector::loginError, this, &Game::connectionError);
    connect(connector, &Connector::messageReceived, this, &Game::messageReceived);
    connect(connector, &Connector::gotTurn, this, &Game::setTurn);
    connect(connector, &Connector::gotField, this, &Game::fieldRecieved);
    connect(connector, &Connector::gameFinished, this, &Game::onlineGameFinished);
    connector->connectToServer(QHostAddress(serverAddress), serverPort.toInt(), playerName);
}

void Game::switchField()
{
    if (curField == field1)
    {
        curField = field2;
        emit curFieldChanged();
    }
    else
    {
        curField = field1;
        emit curFieldChanged();
    }
}

void Game::startInitialization()
{
    emit initializationStarted();
    initPlayer(field1);
}

void Game::setTurn(int isCurrent)
{
    turn = isCurrent;
}

void Game::fieldInitRClicked(int x, int y)
{
    curField->erase(QPoint(x, y));
    drawing = false;
    if (curField->getShipNum() != 10)
        curField->setFilled(false);
}

void Game::fieldInitClicked(int x, int y)
{
    if (!drawing)
    {
        if ((*curField)[x][y] == Cell::State::Unknown && curField->getShipNum() != 10)
        {
            curField->changeCellState(x, y, Cell::State::DrawStart);
            drawing = true;
            bufPos = QPoint(x, y);
        }
    }
    else
    {
        if (!curField->addShip(bufPos, QPoint(x, y)))
            curField->changeCellState(bufPos, Cell::State::Unknown);
        else
        {
            if (curField->getShipNum() == 10)
                curField->setFilled(true);
        }

        drawing = false;
    }
}

void Game::fieldPlayClicked(int x, int y)
{
    if (isOnline)
    {
        if (turn == 1 && sender() == field1)
            connector->sendMove(x, y);
        return;
    }

    if (sender() == curField && (*curField)[x][y] == Cell::State::Unknown)
    {
        if (curField->hit(QPoint(x, y)) == Field::MoveResult::EmptyCell)
            switchField();
        else if (curField->hasLost())
            endLocalGame();
    }
}

void Game::okClicked()
{
    if (isOnline == true)
    {
        finishPlayerInit();
        connector->sendField(field1->shipsToString());
        gameState = GameState::play;
        connect(connector, &Connector::gameStarted, this, &Game::playOnline);
        emit waiting();
    }
    else if (gameState == GameState::initPlayer1)
    {
        finishPlayerInit();
        gameState = GameState::initPlayer2;
        initPlayer(field2);
    }
    else if (gameState == GameState::initPlayer2)
    {
        finishPlayerInit();
        gameState = GameState::play;
        playLocal();
    }
}

void Game::playClicked(bool online)
{
    isOnline = online;
    if (online)
        startOpponentSelection();
    else
        emit nameInputStarted();
}

void Game::namesInputed(const QString& name1, const QString& name2)
{
    field1->setName(name1);
    field2->setName(name2);
    startInitialization();
}

void Game::gameFinished()
{
    endLocalGame();
    emit gameClosed();
}

void Game::randomOpponentClicked(const QString &serverAddress, const QString &serverPort, const QString &playerName)
{
    emit waiting();
    connectToServer(serverAddress, serverPort, playerName);
    connector->lookForOpponent();
    connect(connector, &Connector::opponentFound, this, &Game::startInitialization);
}

void Game::createRoomClicked(const QString &serverAddress, const QString &serverPort, const QString &playerName)
{
    emit waiting();
    connectToServer(serverAddress, serverPort, playerName);
    connect(connector, &Connector::opponentFound, this, &Game::startInitialization);
    connect(connector, &Connector::roomIdRecieved, [this](int id)
    { changeWaitingInfo(QString::number(id)); });
    connector->createRoom();
}

void Game::joinRoomClicked(const QString &serverAddress, const QString &serverPort, const QString &playerName, const QString &roomId)
{
    emit waiting();
    connectToServer(serverAddress, serverPort, playerName);
    connect(connector, &Connector::opponentFound, this, &Game::startInitialization);
    connector->joinRoom(roomId.toInt());
}

void Game::messageReceived(const QString &sender, const QString &text)
{
    qDebug() << sender << " " << text;
}

void Game::connectionError(const QString &error)
{
    qDebug() << "Connection error:\n"
             << error;
}

void Game::fieldRecieved(const QString &cellString, int player)
{
    if (player == 0)
        field1->initCellsFromString(cellString);
    else
        field2->initCellsFromString(cellString);
}

void Game::onlineGameFinished(bool isWinner)
{
    if (isWinner)
        field2->lose();
    else
        field1->lose();
}

void Game::changeWaitingInfo(const QString &info)
{
    waitingInfo = info;
    emit waitingInfoChanged();
}

void Game::exit()
{
    QCoreApplication::exit(0);
}

void Game::returnHome()
{
    field1->DeleteShips();
    field2->DeleteShips();
    emit returnedHome();
}

void Game::restartGame(bool saveNames)
{
    field1->DeleteShips();
    field2->DeleteShips();

    if (saveNames)
        startInitialization();
    else
        emit nameInputStarted();
}

void Game::clearClicked()
{
    curField->DeleteShips();
    curField->setFilled(false);
    drawing = false;
}

void Game::randomClicked()
{
    curField->RandomFill();
    curField->setFilled(true);
    drawing = false;
}

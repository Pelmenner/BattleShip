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
    context = engine.rootContext();
    context->setContextProperty("backend", this);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    mainWindow = engine.rootObjects().first();
    if (!mainWindow)
    {
        qDebug("could not find MainWindow (qml)");
        QCoreApplication::exit(-1);
    }

    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(homePageLoaded()));
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
    disconnect(frontField1, SIGNAL(cellClicked(int, int)), this, SLOT(fieldInitClicked(int, int)));
    disconnect(frontField1, SIGNAL(cellRClicked(int, int)), this, SLOT(fieldInitRClicked(int, int)));
    disconnect(initializer, SIGNAL(randomButtonClicked()), this, SLOT(randomClicked()));
    disconnect(initializer, SIGNAL(clearButtonClicked()), this, SLOT(clearClicked()));
    disconnect(initializer, SIGNAL(okButtonClicked()), this, SLOT(okClicked()));

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

    connect(frontField1, SIGNAL(cellClicked(int, int)), this, SLOT(fieldInitClicked(int, int)));
    connect(frontField1, SIGNAL(cellRClicked(int, int)), this, SLOT(fieldInitRClicked(int, int)));
    connect(initializer, SIGNAL(randomButtonClicked()), this, SLOT(randomClicked()));
    connect(initializer, SIGNAL(clearButtonClicked()), this, SLOT(clearClicked()));
    connect(initializer, SIGNAL(okButtonClicked()), this, SLOT(okClicked()));

    drawing = false;
}

void Game::playLocal()
{
    emit localGameStarted();
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(gameLoaded()));
}

void Game::playOnline()
{
    emit localGameStarted();
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(gameLoaded()));
}

void Game::endLocalGame()
{
    disconnect(frontField1, SIGNAL(cellClicked(int, int)), field1, SIGNAL(clickedEvent(int, int)));
    disconnect(frontField2, SIGNAL(cellClicked(int, int)), field2, SIGNAL(clickedEvent(int, int)));

    disconnect(field1, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int,int)));
    disconnect(field2, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int,int)));

    if (curField == field1)
        field2->showAlive();
    else
        field1->showAlive();

    curField = nullptr;
    emit curFieldChanged();

    //emit gameFinished();
}

void Game::startOpponentSelection()
{
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(opponentSelectPageLoaded()));
    emit opponentSelectionStarted();
}

void Game::connectToServer(const QString& serverAddress, const QString& serverPort, const QString& playerName)
{
    qDebug("Connecting to server...");
    connect(connector, &Connector::loginError, this, &Game::connectionError);
    connect(connector, &Connector::messageReceived, this, &Game::messageReceived);
    connect(connector, &Connector::gotTurn, this, &Game::setTurn);
    connect(connector, &Connector::gotField, this, &Game::fieldRecieved);
    connect(connector, &Connector::gameFinished, this, &Game::onlineGameFinished);
    connector->connectToServer(QHostAddress(serverAddress), serverPort.toInt(), playerName);
}

void Game::startInitialization()
{
    emit initializationStarted();
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(initLoaded()));
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
    qDebug() << "play clicked " << online;
    disconnect(homePage, SIGNAL(playClicked(bool)), this, SLOT(playClicked(bool)));
    isOnline = online;
    if (online)
        startOpponentSelection();
    else
    {
        connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(nameInputLoaded()));
        emit nameInputStarted();
    }
}

void Game::homePageLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(homePageLoaded()));

    homePage = engine.rootObjects().first()->findChild<QObject*>("homePage");
    if (!homePage)
    {
        qDebug("cannot find HomePage (qml)");
        QCoreApplication::exit(-1);
    }

    connect(homePage, SIGNAL(playClicked(bool)), this, SLOT(playClicked(bool)));
}

void Game::initLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(initLoaded()));

    initializer = engine.rootObjects().first()->findChild<QObject*>("initializer");
    frontField1 = engine.rootObjects().first()->findChild<QObject*>("field");

    if (!initializer)
    {
        qDebug("cannot find FieldInitializer (qml)");
        QCoreApplication::exit(-1);
    }

    initPlayer(field1);
}

void Game::gameLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(gameLoaded()));

    frontField1 = engine.rootObjects().first()->findChild<QObject*>("field1");
    frontField2 = engine.rootObjects().first()->findChild<QObject*>("field2");
    frontGame = engine.rootObjects().first()->findChild<QObject*>("game");

    if (!frontGame)
    {
        qDebug("Could not find game (qml)");
        QCoreApplication::exit(-1);
    }
    if (!frontField1 || !frontField2)
    {
        qDebug("Could not find two fields (qml). Check game.qml for field1 and field2");
        QCoreApplication::exit(-1);
    }

    connect(frontField1, SIGNAL(cellClicked(int, int)), field1, SIGNAL(clickedEvent(int, int)));
    connect(frontField2, SIGNAL(cellClicked(int, int)), field2, SIGNAL(clickedEvent(int, int)));

    connect(field1, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int,int)));
    connect(field2, SIGNAL(clickedEvent(int, int)), this, SLOT(fieldPlayClicked(int,int)));

    connect(frontGame, SIGNAL(finished()), this, SLOT(gameFinished()));

    curField = field1;
    emit curFieldChanged();
}

void Game::nameInputLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(nameInputLoaded()));
    namesInput = engine.rootObjects().first()->findChild<QObject*>("namesInput");

    if (!namesInput)
    {
        qDebug("Could not find namesInput (qml)");
        QCoreApplication::exit(-1);
    }

    connect(namesInput, SIGNAL(completed()), this, SLOT(namesInputed()));
}

void Game::namesInputed()
{
    disconnect(namesInput, SIGNAL(completed()), this, SLOT(namesInputed()));
    field1->setName(namesInput->property("name1").toString());
    field2->setName(namesInput->property("name2").toString());
    startInitialization();
}

void Game::gameFinished()
{
    disconnect(frontGame, SIGNAL(finished()), this, SLOT(gameFinished()));
    endLocalGame();
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(restartPageLoaded()));
    emit gameClosed();
}

void Game::restartPageLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(restartPageLoaded()));
    restartPage = engine.rootObjects().first()->findChild<QObject*>("restartPage");
    if (!restartPage)
    {
        qDebug("Could not find restartPage (qml)");
        QCoreApplication::exit(-1);
    }
    connect(restartPage, SIGNAL(restartClicked(bool)), this, SLOT(gameRestarted(bool)));
    connect(restartPage, SIGNAL(exitClicked()), this, SLOT(exitClicked()));
    connect(restartPage, SIGNAL(homeClicked()), this, SLOT(homeClicked()));
}

void Game::opponentSelectPageLoaded()
{
    disconnect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(opponentSelectPageLoaded()));
    opponentSelectPage = engine.rootObjects().first()->findChild<QObject*>("opponentSelectPage");
    if (!opponentSelectPage)
    {
        qDebug("Could not find opponentSelectPage (qml)");
        QCoreApplication::exit(-1);
    }
    connect(opponentSelectPage, SIGNAL(randomClicked(QString,  QString, QString)),
            this, SLOT(randomOpponentClicked(QString, QString, QString)));
    connect(opponentSelectPage, SIGNAL(createClicked(QString, QString, QString)),
            this, SLOT(createRoomClicked(QString, QString, QString)));
    connect(opponentSelectPage, SIGNAL(joinClicked(QString, QString, QString, QString)),
            this, SLOT(joinRoomClicked(QString, QString, QString, QString)));
    qDebug("Opponent selection...");
}

void Game::waitingPageLoaded()
{
}

void Game::randomOpponentClicked(const QString& serverAddress, const QString& serverPort, const QString& playerName)
{
    emit waiting();
    connectToServer(serverAddress, serverPort, playerName);
    connector->lookForOpponent();
    connect(connector, &Connector::opponentFound, this, &Game::startInitialization);
}

void Game::createRoomClicked(const QString &serverAddress, const QString &serverPort, const QString& playerName)
{
    emit waiting();
    connectToServer(serverAddress, serverPort, playerName);
    connect(connector, &Connector::opponentFound, this, &Game::startInitialization);
    connect(connector, &Connector::roomIdRecieved, [this](int id){changeWaitingInfo(QString::number(id));});
    connector->createRoom();
}

void Game::joinRoomClicked(const QString &serverAddress, const QString &serverPort, const QString& playerName, const QString &roomId)
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
    qDebug() << "Connection error:\n" << error;
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

void Game::exitClicked()
{
    QCoreApplication::exit(0);
}

void Game::returnHome()
{
    connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(homePageLoaded()));
    field1->DeleteShips();
    field2->DeleteShips();
    emit returnedHome();
}

void Game::gameRestarted(bool saveNames)
{
    disconnect(restartPage, SIGNAL(restartClicked(bool)), this, SLOT(gameRestarted(bool)));
    field1->DeleteShips();
    field2->DeleteShips();

    if (saveNames)
    {
        startInitialization();
    }
    else
    {
        connect(mainWindow, SIGNAL(loadCompleted()), this, SLOT(nameInputLoaded()));
        emit nameInputStarted();
    }
}

void Game::clearClicked()
{
    if (curField)
        curField->DeleteShips();
    curField->setFilled(false);
    drawing = false;
}

void Game::randomClicked()
{
    if (curField)
        curField->RandomFill();
    curField->setFilled(true);
    drawing = false;
}

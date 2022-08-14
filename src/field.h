#ifndef FIELD_H
#define FIELD_H

#include "cell.h"

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QQmlApplicationEngine>
#include <QAbstractTableModel>
#include <QQmlExtensionPlugin>

class Field : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(bool filled READ isFilled NOTIFY filledChanged)
    Q_PROPERTY(int health READ getTotalHealth NOTIFY healthChanged)
    Q_PROPERTY(ShipCounter* shipCounter READ getShipCounter CONSTANT)

public:
    struct ship
    {
        int length;
        enum class Direction
        {
            vertical,
            horizontal
        };
        Direction direction;
        QPoint pos;
        int health;

        ship(QPoint start, QPoint end);
        ship() : length(0), direction(Direction::vertical), health(0){};
        bool containsCell(const QPoint& cell) const;
    };

    enum class MoveResult
    {
        WrongMove,
        EmptyCell,
        Damaged
    };

    class ShipCounter : public QAbstractListModel
    {
    public:
        enum CounterRoles {
                Length = Qt::UserRole + 1,
                Count
            };

        explicit ShipCounter(QObject *parent = nullptr) {shipCount.resize(4);};
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        void update(int index, int value);
        void erase(int length);
        void add(int length);
        void reset();
        int get(int length) const;
        int total() const;
        void updateFrom(const ShipCounter&);
        QHash<int, QByteArray> roleNames() const override;
    private:
        QVector<int> shipCount;
    };

    explicit Field(QObject *parent = nullptr);
    Field(const Field &f);

    Q_INVOKABLE Cell *getCell(int x, int y);

    QString shipsToString();
    bool initCellsFromString(const QString &cellsString);
    QString cellsToString() const;

    void setName(const QString &newName);
    QString getName() const;

    // erases ship containing current point (cell)
    bool erase(QPoint pos);

    // operations with cells
    void clearCells();
    void updateCells();
    void showAlive();
    void lose();

    void changeCellState(const QPoint &p, const Cell::State state);
    void changeCellState(int x, int y, const Cell::State state);

    void RandomFill();
    void DeleteShips();

    MoveResult hit(const QPoint &position);
    int getShipNum() const;

    bool hasLost() const;
    bool addShip(QPoint begin, QPoint end);

    // are all the ships (10) placed
    bool isFilled() const;
    void setFilled(bool newFilled); // better do it private

    int getTotalHealth() const;

    //get row of cells
    QVector<Cell::State> &operator[](int index);

    ShipCounter* getShipCounter();

signals:
    void clickedEvent(int, int);
    void rClickedEvent(int, int);
    void filledChanged();
    void nameChanged();
    void healthChanged();

private:
    QVector<QVector<Cell *>> cells;
    QVector<QVector<Cell::State>> cellStates;
    ShipCounter shipCounter;
    QVector<ship> ships;
    QString name;

    bool filled;
    bool lost;

    std::optional<int> checkHit(const QPoint &position) const;
    void showAndSurroundKilled(const Field::ship& killedShip);
    void getNeighbours(QPoint pos, QVector<QPoint> &ans);
};

Q_DECLARE_METATYPE(Field)

#endif // FIELD_H

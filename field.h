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
    Q_PROPERTY(QVector<int> shipCount READ getShipCount NOTIFY shipCountChanged)

public:
    struct ship
    {
        int length;
        enum class Direction{vertical, horizontal};
        Direction direction;
        QPoint pos;
        int health;

        ship(QPoint start, QPoint end);
        ship() : length(0), direction(Direction::vertical), health(0) {};
    };

    explicit Field(QObject *parent = nullptr);
    Field(const Field& f);

    Q_INVOKABLE Cell* getCell(int x, int y);

    void setName(const QString &newName);
    bool erase(QPoint pos);
    void clearCells();
    void updateCells();
    void showAlive();

    void changeCellState(const QPoint& p, const Cell::CellState state);
    void changeCellState(int x, int y, const Cell::CellState state);

    void RandomFill();
    void DeleteShips();

    int hit(const QPoint &position);
    int getShipNum() const;

    bool hasLost() const;
    bool addShip(QPoint begin, QPoint end);

    bool isFilled();
    void setFilled(bool newFilled);

    int getTotalHealth();

    QString getName() const;

    QVector<int> getShipCount();
    QVector<Cell::CellState> &operator[](int index);

signals:
    void clickedEvent(int, int);
    void rClickedEvent(int, int);
    void filledChanged();
    void nameChanged();
    void healthChanged();
    void shipCountChanged();

private:
    QQmlApplicationEngine engine;

    QVector <QVector <Cell*>> cells;
    QVector<QVector<Cell::CellState>> cellStates;
    QVector <int> count_ships;
    QVector <ship> ships;
    QString name;

    bool filled;

    int checkHit(const QPoint& position) const;
    void showAndSurroundKilled(int index);
    void getNeighbours(QPoint pos, QVector<QPoint> &ans);
};

Q_DECLARE_METATYPE(Field)

#endif // FIELD_H

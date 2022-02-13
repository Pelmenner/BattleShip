#ifndef CELL_H
#define CELL_H

#include <QObject>

class QMouseEvent;

class Cell : public QObject
{
    Q_OBJECT

    Q_PROPERTY(State state READ getState NOTIFY stateChanged)

public:
    enum class State
    {
        Unknown,
        Hit,
        Checked,
        AutoChecked,
        Shown,
        DrawStart //first cell when placing a ship
    };
    Q_ENUM(State)

    explicit Cell(QObject *parent = 0, int x = 0, int y = 0);
    void changeState(State newState);
    State getState() const;

signals:
    void stateChanged();

private:
    int x, y;
    State state;
};

#endif // CELL_H

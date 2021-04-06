#ifndef CELL_H
#define CELL_H

#include <QObject>

class QMouseEvent;

class Cell : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CellState state READ getState NOTIFY stateChanged)

public:
    enum class CellState
    {
        Unknown,
        Hit,
        Checked,
        AutoChecked,
        Shown,
        DrawStart //first cell when placing a ship
    };
    Q_ENUM(CellState)

    explicit Cell(QObject *parent = 0, int x = 0, int y = 0);
    void changeState(CellState newState);
    CellState getState() const;

signals:
    void stateChanged();

private:
    int x, y;
    CellState state;
};

#endif // CELL_H

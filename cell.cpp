#include "cell.h"
#include "colors.h"

#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QMap>
#include <QColor>

Cell::Cell(QObject *parent, int x, int y) :
    QObject(parent), x(x), y(y), state(State::Unknown)
{}

void Cell::changeState(Cell::State newState)
{
    state = newState;
    emit stateChanged();
}

Cell::State Cell::getState() const
{
    return state;
}

#include "cell.h"

#include <QDebug>

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

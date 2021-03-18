#include "cell.h"
#include "colors.h"

#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QMap>
#include <QColor>

QMap<Cell::CellState, QColor> Cell::stateColorMapper = QMap<Cell::CellState, QColor>();

Cell::Cell(QObject *parent, int x, int y) :
    QObject(parent), x(x), y(y), state(CellState::Unknown)
{
    stateColorMapper[CellState::Hit] = Color::red;
    stateColorMapper[CellState::Shown] = Color::green;
    stateColorMapper[CellState::Unknown] = Color::black;
    stateColorMapper[CellState::Checked] = Color::gray;
    stateColorMapper[CellState::AutoChecked] = Color::gray;
    stateColorMapper[CellState::DrawStart] = Color::yellow;
}

void Cell::changeState(Cell::CellState newState)
{
    state = newState;
    emit colorChanged();
}

Cell::CellState Cell::getState() const
{
    return state;
}

QString Cell::getColor() const
{
    return stateColorMapper[state].name();
}

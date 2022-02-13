#include "field.h"

#include <QTime>
#include <QRandomGenerator>

Field::Field(QObject *parent) : QObject(parent), filled(false), lost(false)
{
    count_ships.resize(4);
    cells.fill(QVector<Cell *>(10), 10);
    cellStates.fill(QVector<Cell::State>(10, Cell::State::Unknown), 10);

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            cells[i][j] = new Cell(this, i, j);
}

Field::Field(const Field &f) : cells(f.cells), cellStates(f.cellStates),
                               count_ships(f.count_ships), name(f.name), filled(f.filled), lost(f.lost) {}

Cell *Field::getCell(int x, int y)
{
    return cells[x][y];
}

int Field::checkHit(const QPoint &position) const
{
    for (int i = 0; i < ships.size(); ++i)
    {
        if (ships[i].containsCell(position))
            return i;
    }
    return -1;
}

void Field::showAndSurroundKilled(int index)
{
    QPoint start = ships[index].pos;
    QPoint end;
    end.rx() = start.x() + ((static_cast<int>(ships[index].direction) + 1) % 2) * ships[index].length;
    end.ry() = start.y() + static_cast<int>(ships[index].direction) * ships[index].length;

    QPoint curPos = start;
    QVector<QPoint> cellsNearKilled;
    while (curPos != end)
    {
        changeCellState(curPos, Cell::State::Hit);
        getNeighbours(curPos, cellsNearKilled);
        curPos.rx() += (static_cast<int>(ships[index].direction) + 1) % 2;
        curPos.ry() += static_cast<int>(ships[index].direction) % 2;
    }

    for (const auto &coordinates : cellsNearKilled)
        if (cellStates[coordinates.x()][coordinates.y()] == Cell::State::Unknown)
            changeCellState(coordinates, Cell::State::AutoChecked);
}

void Field::getNeighbours(QPoint pos, QVector<QPoint> &ans)
{
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            const int x = pos.x() + dx;
            const int y = pos.y() + dy;
            if (x >= 0 && y >= 0 && x < 10 && y < 10 && (dx || dy))
                ans.emplace_back(x, y);
        }
    }
}

bool Field::ship::containsCell(const QPoint &cell) const
{
    if (direction == ship::Direction::vertical)
    {
        if (cell.y() != this->pos.y())
            return false;
        if (cell.x() < this->pos.x() || cell.x() > this->pos.x() + this->length - 1)
            return false;
    }
    else
    {
        if (cell.x() != this->pos.x())
            return false;
        if (cell.y() < this->pos.y() || cell.y() > this->pos.y() + this->length - 1)
            return false;
    }
    return true;
}

int Field::getTotalHealth() const
{
    if (lost)
        return 0;

    int totalHealth = 4 + 3 * 2 + 2 * 3 + 4;
    for (const auto &ship : ships)
        totalHealth -= ship.length - ship.health;
    return totalHealth;
}

bool Field::erase(QPoint pos)
{
    bool found = false;
    for (int i = 0; i < ships.size(); ++i)
    {
        if (ships[i].containsCell(pos))
        {
            count_ships[ships[i].length - 1]--;
            ships.erase(ships.cbegin() + i);
            found = true;
            break;
        }
    }
    updateCells();
    emit shipCountChanged();
    return found;
}

void Field::clearCells()
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            changeCellState(i, j, Cell::State::Unknown);
}

void Field::setName(const QString &newName)
{
    name = newName;
    emit nameChanged();
}

void Field::updateCells()
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            changeCellState(i, j, Cell::State::Unknown);

    for (int i = 0; i < ships.size(); i++)
        showAndSurroundKilled(i);
}

void Field::showAlive()
{
    for (const auto &ship : ships)
    {
        QPoint start = ship.pos;
        QPoint end = start;
        end.rx() += ((static_cast<int>(ship.direction) + 1) % 2) * ship.length;
        end.ry() += static_cast<int>(ship.direction) * ship.length;
        while (start != end)
        {
            if (cellStates[start.x()][start.y()] == Cell::State::Unknown)
                changeCellState(start, Cell::State::Shown);

            start.rx() += (static_cast<int>(ship.direction) + 1) % 2;
            start.ry() += static_cast<int>(ship.direction);
        }
    }
}

void Field::lose()
{
    lost = true;
    emit healthChanged();
}

void Field::changeCellState(const QPoint &p, const Cell::State state)
{
    cells[p.x()][p.y()]->changeState(state);
    cellStates[p.x()][p.y()] = state;
}

void Field::changeCellState(int x, int y, const Cell::State state)
{
    cells[x][y]->changeState(state);
    cellStates[x][y] = state;
}

Field::MoveResult Field::hit(const QPoint &position)
{
    if (cellStates[position.x()][position.y()] != Cell::State::Unknown)
        return MoveResult::WrongMove;

    int res = checkHit(position);
    changeCellState(position, Cell::State::Checked);
    if (res == -1)
        return MoveResult::EmptyCell;

    changeCellState(position, Cell::State::Hit);
    --ships[res].health;
    emit healthChanged();

    if (ships[res].health == 0)
        showAndSurroundKilled(res);
    return MoveResult::Damaged;
}

int Field::getShipNum() const
{
    return count_ships[0] + count_ships[1] +
           count_ships[2] + count_ships[3];
}

bool Field::hasLost() const
{
    for (const auto &ship : ships)
        if (ship.health)
            return false;

    return true;
}

// TODO: needs refactoring
bool Field::addShip(QPoint begin, QPoint end)
{
    if (begin.x() > end.x() || begin.y() > end.y())
        qSwap(begin, end);
    if (end.x() - begin.x() && end.y() - begin.y())
        return false;

    int direction = (int)(end.y() != begin.y());
    int length = (end.x() - begin.x()) + (end.y() - begin.y()) + 1;

    if (length > 4)
        return false;
    if (count_ships[length - 1] == 4 - length + 1)
        return false;

    QPoint curPos = begin;
    while (curPos != end)
    {
        if (cellStates[curPos.x()][curPos.y()] == Cell::State::AutoChecked)
            return false;
        curPos.rx() += (direction + 1) % 2;
        curPos.ry() += direction % 2;
    }
    if (cellStates[curPos.x()][curPos.y()] == Cell::State::AutoChecked)
        return false;

    ships.push_back(ship(begin, end));
    count_ships[length - 1]++;

    curPos = begin;
    QVector<QPoint> currCells;
    while (curPos != end)
    {
        getNeighbours(curPos, currCells);
        changeCellState(curPos, Cell::State::Hit);
        curPos.rx() += (direction + 1) % 2;
        curPos.ry() += direction % 2;
    }
    changeCellState(curPos, Cell::State::Hit);
    getNeighbours(curPos, currCells);

    for (auto coordinates : currCells)
        if (cellStates[coordinates.x()][coordinates.y()] == Cell::State::Unknown)
            changeCellState(coordinates, Cell::State::AutoChecked);

    emit shipCountChanged();
    setFilled(getShipNum() == 10);

    return true;
}

bool Field::isFilled() const
{
    return filled;
}

void Field::setFilled(bool newFilled)
{
    filled = newFilled;
    emit filledChanged();
}

void Field::RandomFill()
{
    DeleteShips();
    for (int len = 4; len > 0; --len)
    {
        int shipsLeft = 5 - len;
        while (shipsLeft != 0)
        {
            QPoint begin(QRandomGenerator::global()->bounded(10), QRandomGenerator::global()->bounded(10));
            if (cellStates[begin.x()][begin.y()] != Cell::State::Unknown)
                continue;

            int dir = QRandomGenerator::global()->bounded(2);
            QPoint end(begin.x() + (!dir) * (len - 1), begin.y() + dir * (len - 1));
            if (end.x() >= 10 || end.y() >= 10)
                continue;
            if (cellStates[end.x()][end.y()] != Cell::State::Unknown)
                continue;

            if (addShip(begin, end))
                --shipsLeft;
        }
    }
    setFilled(true);
    emit shipCountChanged();
}

void Field::DeleteShips()
{
    ships.clear();
    count_ships.fill(0, 4);
    clearCells();
    emit shipCountChanged();
    setFilled(false);
}

QString Field::getName() const
{
    return name;
}

QVector<int> Field::getShipCount()
{
    return count_ships;
}

QVector<Cell::State> &Field::operator[](int index)
{
    return cellStates[index];
}

Field::ship::ship(QPoint start, QPoint end)
{
    if (start.x() > end.x() || start.y() > end.y())
        qSwap(start, end);

    health = length = (end.x() - start.x()) + (end.y() - start.y()) + 1;
    direction = ship::Direction(static_cast<int>(end.y() != start.y()));
    pos = start;
}

QString Field::shipsToString()
{
    QString result;
    for (const auto &ship : ships)
    {
        result.append(QChar('0' + ship.pos.x()));
        result.push_back(QChar(char('0' + ship.pos.y())));
        result.push_back(QChar('0' + ship.length));
        result.push_back(QChar(ship.direction == ship::Direction::vertical ? '1' : '0'));
    }
    return result;
}

bool Field::initCellsFromString(const QString &cellsString)
{
    if (cellsString.length() != 10 * 10)
        return false;

    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            switch (cellsString[i * 10 + j].unicode())
            {
            case 'U':
                cells[i][j]->changeState(Cell::State::Unknown);
                break;
            case 'H':
                cells[i][j]->changeState(Cell::State::Hit);
                break;
            case 'C':
                cells[i][j]->changeState(Cell::State::Checked);
                break;
            case 'A':
                cells[i][j]->changeState(Cell::State::AutoChecked);
                break;
            case 'S':
                cells[i][j]->changeState(Cell::State::Shown);
                break;
            case 'D': // probably shouldn't be used
                cells[i][j]->changeState(Cell::State::DrawStart);
                break;
            default:
                return false;
            }
        }
    }
    return true;
}

QString Field::cellsToString() const
{
    QString result;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            switch (cells[i][j]->getState())
            {
            case Cell::State::AutoChecked:
                result.push_back('A');
                break;
            case Cell::State::Checked:
                result.push_back('C');
                break;
            case Cell::State::DrawStart:
                result.push_back('D');
                break;
            case Cell::State::Hit:
                result.push_back('H');
                break;
            case Cell::State::Shown:
                result.push_back('S');
                break;
            case Cell::State::Unknown:
                result.push_back('U');
                break;
            }
        }
    }
    return result;
}

#ifndef CELL_H
#define CELL_H

#include <QObject>

class QMouseEvent;

class Cell : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString color READ getColor NOTIFY colorChanged)

public:
    enum class CellState
    {
        Unknown,
        Hit,
        Checked,
        AutoChecked,
        Shown,
        DrawStart
    };

    explicit Cell(QObject *parent = 0, int x = 0, int y = 0);
    void changeState(CellState newState);
    QString getColor() const;

signals:
    void rightClicked(int, int);
    void leftClicked(int, int);
    void colorChanged();

private:
    int x, y;
    static QMap<CellState, QColor> stateColorMapper;
    CellState state;
};

#endif // CELL_H

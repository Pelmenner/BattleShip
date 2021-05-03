#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

// not used now

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
public:
    explicit Player(QObject *parent = nullptr);

    QString getName() {return name;};
    void setName(const QString& newName) {emit nameChanged();name = newName;};
signals:
    void nameChanged();

private:
    QString name;
    int id;

};

#endif // PLAYER_H

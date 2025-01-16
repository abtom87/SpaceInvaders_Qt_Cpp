#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsItem>
#include <QKeyEvent>

#include "settings.h"

enum class EColor
{
    Red,
    Pink,
    Blue
};

class CCannon : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    CCannon(EColor color, QGraphicsItem *pParent = nullptr);

    void Shoot();

    EColor GetColor() const;
    void SetColor(EColor ecol);

signals:
    void sigIncreaseScore();
    void sigDecreaseScore();

private:
    EColor m_eColor;
    QString m_Path_RedCannon = "../Resources/RedCannon.png";
    QString m_Path_PinkCannon = "../Resources/PinkCannon.png";
    QString m_Path_BlueCannon = "../Resources/BlueCannon.png";
};

/************************************************** Component ALIEN ************************************************** */

class CAlien : public QObject, public QGraphicsPixmapItem
{

    Q_OBJECT

public:
    CAlien(EColor EColor, QGraphicsItem *pParent = nullptr);

    EColor GetColor() const;
    void SetColor(EColor ecol);

signals:
    void sigGameOver();
    void sigDecreaseHealth();

public slots:
    void onMove();

private:
    EColor m_eColor;

    QString m_Path_RedAlien = "../Resources/RedCannon.png";
    QString m_Path_PinkAlien = "../Resources/PinkAlien.png";
    QString m_Path_BlueAlien = "../Resources/BlueAlien.png";
};

/************************************************** Component BULLET ************************************************** */

class CBullet : public QObject, public QGraphicsPixmapItem
{

    Q_OBJECT

public:
    CBullet(EColor eColor, QGraphicsItem *pParent = nullptr);
    EColor GetColor() const;
    void SetColor(EColor ecol);

signals:
    void sigIncreaseScore();
    void sigDecreaseScore();

public slots:
    void onMove();

private:
    EColor m_eColor;

    QString m_Path_RedBullet = "../Resources/RedBullet.png";
    QString m_Path_PinkBullet = "../Resources/PinkBullet.png";
    QString m_Path_BlueBullet = "../Resources/BlueBullet.png";
};
class CBullet;

/************************************************** Component Points ************************************************** */
class CPoints : public QGraphicsTextItem
{
public:
    CPoints(QGraphicsTextItem *pParent = nullptr);

    void IncreaseScore();
    void DecreaseScore();
    void DecreaseHealth();

    int GetHealth() const;
    int GetScore() const;
    void Reset();

private:
    int m_Health = gMaxHealth;
    int m_nScore = 0;
};
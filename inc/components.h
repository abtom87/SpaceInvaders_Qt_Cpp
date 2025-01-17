#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>

#include "settings.h"

enum class EColor { Red, Pink, Blue };
class CBullet;

class CCannon : public QObject, public QGraphicsPixmapItem {
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

  QString m_Path_RedCannon = g_vars::gParFolderPath + "RedCannon.png";
  QString m_Path_PinkCannon = g_vars::gParFolderPath + "PinkCannon.png";
  QString m_Path_BlueCannon = g_vars::gParFolderPath + "BlueCannon.png";
  std::unique_ptr<CBullet> pBullet;
};

/************************************************** Component ALIEN
 * ************************************************** */

class CAlien : public QObject, public QGraphicsPixmapItem {

  Q_OBJECT

public:
  CAlien(EColor EColor, QGraphicsItem *pParent = nullptr);
  ~CAlien();

  EColor GetColor() const;
  void SetColor(EColor ecol);

signals:
  void sigGameOver();
  void sigDecreaseHealth();

public slots:
  void onMove();

private:
  EColor m_eColor;

  QString m_Path_RedAlien = g_vars::gParFolderPath + "RedAlien.png";
  QString m_Path_PinkAlien = g_vars::gParFolderPath + "PinkAlien.png";
  QString m_Path_BlueAlien = g_vars::gParFolderPath + "BlueAlien.png";

  std::unique_ptr<QTimer> m_pAlienTimer;
};

/************************************************** Component BULLET
 * ************************************************** */

class CBullet : public QObject, public QGraphicsPixmapItem {

  Q_OBJECT

public:
  CBullet(EColor eColor, QGraphicsItem *pParent = nullptr);
  ~CBullet();
  EColor GetColor() const;
  void SetColor(EColor ecol);

signals:
  void sigIncreaseScore();
  void sigDecreaseScore();

public slots:
  void onMove();

private:
  EColor m_eColor;

  QString m_Path_RedBullet = g_vars::gParFolderPath + "RedBullet.png";
  QString m_Path_PinkBullet = g_vars::gParFolderPath + "PinkBullet.png";
  QString m_Path_BlueBullet = g_vars::gParFolderPath + "BlueBullet.png";

  std::unique_ptr<QTimer> m_pBulletTimer;

  void startTimer(uint16_t bulletSpeed);
  void stopTimer();
};

/************************************************** Component Points
 * ************************************************** */
class CPoints : public QGraphicsTextItem {
public:
  CPoints(QGraphicsTextItem *pParent = nullptr);

  void IncreaseScore();
  void DecreaseScore();
  void DecreaseHealth();

  int GetHealth() const;
  int GetScore() const;
  void Reset();

private:
  int m_Health = g_vars::gMaxHealth;
  int m_nScore = 0;
};
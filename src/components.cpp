#include "inc/components.h"

#include <QGraphicsScene>
#include <QTimer>
#include <iostream>

CCannon::CCannon(EColor color, QGraphicsItem *pParent)
    : QGraphicsPixmapItem(pParent) {
  SetColor(color);
}

void CCannon::Shoot() {
  CBullet *pBullet = new CBullet(m_eColor);
  connect(pBullet, &CBullet::sigIncreaseScore, this,
          &CCannon::sigIncreaseScore);
  connect(pBullet, &CBullet::sigDecreaseScore, this,
          &CCannon::sigDecreaseScore);

  pBullet->setPos(x() + 15, y() - 10);
  scene()->addItem(pBullet);
}

EColor CCannon::GetColor() const { return m_eColor; }

void CCannon::SetColor(EColor ecol) {
  m_eColor = ecol;

  switch (ecol) {

  case EColor::Red: {
    QPixmap oPixMap(m_Path_RedCannon);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  case EColor::Pink: {
    QPixmap oPixMap(m_Path_PinkCannon);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  case EColor::Blue: {
    QPixmap oPixMap(m_Path_BlueCannon);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  default: {
    QPixmap oPixMap(m_Path_RedCannon);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
    m_eColor = EColor::Red;
  } break;
  }
}

/************************* Component ALIEN************************** */

CAlien::CAlien(EColor eColor, QGraphicsItem *pParent)
    : QGraphicsPixmapItem(pParent) {
  SetColor(eColor);

  m_pTimer = new QTimer();

  connect(m_pTimer, &QTimer::timeout, this, &CAlien::onMove);
  m_pTimer->start(g_vars::gAlienSpeed);
}
CAlien::~CAlien() {
  m_pTimer->stop();
  delete m_pTimer;
}

EColor CAlien::GetColor() const { return m_eColor; }

void CAlien::SetColor(EColor ecol) {
  m_eColor = ecol;

  switch (ecol) {

  case EColor::Red: {
    QPixmap oPixMap(m_Path_RedAlien);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  case EColor::Pink: {
    QPixmap oPixMap(m_Path_PinkAlien);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  case EColor::Blue: {
    QPixmap oPixMap(m_Path_BlueAlien);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;

  default: {
    QPixmap oPixMap(m_Path_RedAlien);
    setPixmap(oPixMap.scaled(QSize(100, 100), Qt::KeepAspectRatio));
  } break;
  }
}

void CAlien::onMove() {
  setPos(x(), y() + 5);

  if (pos().y() >= (scene()->height() - g_vars::gCannonSize.height())) {
    scene()->removeItem(this);
    emit sigDecreaseHealth();
    delete this;
  }

  QList<QGraphicsItem *> lstCollidingItems = collidingItems();

  // If aliens collide with cannon, game over
  for (auto const pItem : lstCollidingItems) {
    if (dynamic_cast<CCannon *>(pItem))
      emit sigGameOver();
  }
}

/************************** Component BULLET*********************************/

CBullet::CBullet(EColor eColor, QGraphicsItem *pParent)
    : QGraphicsPixmapItem(pParent) {
  SetColor(eColor);

  m_pTimer = new QTimer();

  connect(m_pTimer, &QTimer::timeout, this, &CBullet::onMove);

  startTimer(g_vars::gBulletSpeed);
}

void CBullet::startTimer(uint16_t bulletSpeed) { m_pTimer->start(bulletSpeed); }

void CBullet::stopTimer(void) { m_pTimer->stop(); }

CBullet::~CBullet() {

  stopTimer();
  delete m_pTimer;
}

EColor CBullet::GetColor() const { return m_eColor; }

void CBullet::SetColor(EColor ecol) {
  m_eColor = ecol;
  switch (ecol) {

  case EColor::Red: {
    QPixmap oPixMap(m_Path_RedBullet);
    setPixmap(oPixMap.scaled(QSize(40, 40), Qt::KeepAspectRatio));
  } break;

  case EColor::Pink: {
    QPixmap oPixMap(m_Path_PinkBullet);
    setPixmap(oPixMap.scaled(QSize(40, 40), Qt::KeepAspectRatio));
  } break;

  case EColor::Blue: {
    QPixmap oPixMap(m_Path_BlueBullet);
    setPixmap(oPixMap.scaled(QSize(40, 40), Qt::KeepAspectRatio));
  } break;

  default: {
    QPixmap oPixMap(m_Path_RedBullet);
    setPixmap(oPixMap.scaled(QSize(40, 40), Qt::KeepAspectRatio));
  } break;
  }
}

void CBullet::onMove() {

  QList<QGraphicsItem *> lstCollidingItems = collidingItems();

  // If bullet collides with aline, remove bullet and alien from scene.
  for (auto const pItem : lstCollidingItems) {

    CAlien *pAlien = dynamic_cast<CAlien *>(pItem);
    if (pAlien != nullptr) {

      if (pAlien->GetColor() == GetColor()) {

        scene()->removeItem(pAlien);
        scene()->removeItem(this);

        emit sigIncreaseScore();

        delete pAlien;
        delete this;
      } else {
        emit sigDecreaseScore();
        scene()->removeItem(this);
        delete this;
      }
      return;
    }
  }

  setPos(x(), y() - g_vars::gBulletDistance);

  if (pos().y() < 0) {
    scene()->removeItem(this);
    delete this;
  }
}

/************************** Component Points ************************** */

CPoints::CPoints(QGraphicsTextItem *pParent) : QGraphicsTextItem(pParent) {

  setPlainText(QString("Health: ") + QString::number(m_Health) + "\n" +
               QString("Score: ") + QString::number(m_nScore));
  setDefaultTextColor(Qt::red);
  setFont(QFont("times", 24));
}

void CPoints::IncreaseScore() {
  m_nScore += 50;
  setPlainText(QString("Health: ") + QString::number(m_Health) + "\n" +
               QString("Score: ") + QString::number(m_nScore));
}

void CPoints::DecreaseScore() {
  m_nScore -= 50;
  setPlainText(QString("Health: ") + QString::number(m_Health) + "\n" +
               QString("Score: ") + QString::number(m_nScore));
}

void CPoints::DecreaseHealth() {
  m_Health--;
  setPlainText(QString("Health: ") + QString::number(m_Health) + "\n" +
               QString("Score: ") + QString::number(m_nScore));
}

int CPoints::GetHealth() const { return m_Health; }

int CPoints::GetScore() const { return m_nScore; }

void CPoints::Reset() {
  m_nScore = 0;
  m_Health = g_vars::gMaxHealth;
  setPlainText(QString("Health: ") + QString::number(m_Health) + "\n" +
               QString("Score: ") + QString::number(m_nScore));
}

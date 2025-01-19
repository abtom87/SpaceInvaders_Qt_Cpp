#include "inc/components.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>
#include <iostream>

CCannon::CCannon(EColor color, QGraphicsItem *pParent)
    : QGraphicsPixmapItem(pParent) {
  SetColor(color);

  // FIXME
  m_pUpdateBulletsTimer = new QTimer();

  // std::make_unique<QTimer>();

  connect(m_pUpdateBulletsTimer, &QTimer::timeout, this,
          &CCannon::updateBullets);
  m_pUpdateBulletsTimer->start(50);
}

void CCannon::updateBullets() {
  // Delete bullet in vector if marked for deletion
  for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
    if ((*it)->IsMarkedForDeletion()) {
      it = m_Bullets.erase(it); // Erase returns the next valid iterator
    } else {
      ++it; // Move to the next element
    }
  }
}

void CCannon::Shoot() {

  auto pBullet = std::make_unique<CBullet>(m_eColor);

  connect(pBullet.get(), &CBullet::sigIncreaseScore, this,
          &CCannon::sigIncreaseScore);
  connect(pBullet.get(), &CBullet::sigDecreaseScore, this,
          &CCannon::sigDecreaseScore);

  pBullet->setPos(x() + 15, y() - 10);
  scene()->addItem(pBullet.get());

  // Collect bullet in vector of bullets
  m_Bullets.push_back(std::move(pBullet));
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

  m_pAlienTimer = std::make_unique<QTimer>();

  connect(m_pAlienTimer.get(), &QTimer::timeout, this, &CAlien::onMove);
  m_pAlienTimer->start(g_vars::gAlienSpeed);
}
CAlien::~CAlien() {
  m_pAlienTimer->stop();
  m_pAlienTimer.reset();
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

  // Ensure this object is in a valid scene
  if (!scene()) {

    return; // Safely exit if scene() is null
  }

  setPos(x(), y() + 5);

  if (pos().y() >= (scene()->height() - g_vars::gCannonSize.height())) {

    scene()->removeItem(this);
    emit sigDecreaseHealth();

    deleteLater();
    return;
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
  m_markedForDeletion = false;

  m_pBulletTimer = std::make_unique<QTimer>();

  connect(m_pBulletTimer.get(), &QTimer::timeout, this, &CBullet::onMove);

  startTimer(g_vars::gBulletSpeed);
}

bool CBullet::IsMarkedForDeletion() { return m_markedForDeletion; }

void CBullet::startTimer(uint16_t bulletSpeed) {
  m_pBulletTimer->start(bulletSpeed);
}

void CBullet::stopTimer(void) { m_pBulletTimer->stop(); }

CBullet::~CBullet() {

  stopTimer();
  m_pBulletTimer.reset();
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

  for (auto const pItem : lstCollidingItems) {
    CAlien *pAlien = dynamic_cast<CAlien *>(pItem);
    if (pAlien != nullptr) {
      if (pAlien->GetColor() == GetColor()) {
        scene()->removeItem(pAlien);
        emit sigIncreaseScore();
        pAlien->deleteLater(); // Mark alien for deletion
      } else {
        if (!m_markedForDeletion) {

          emit sigDecreaseScore();
        }
      }
      m_markedForDeletion = true; // Mark this bullet for deletion
      return;
    }
  }

  setPos(x(), y() - g_vars::gBulletDistance);

  if (pos().y() < 0) {
    m_markedForDeletion = true; // Mark this bullet for deletion
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

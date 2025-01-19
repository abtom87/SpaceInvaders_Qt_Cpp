#include "inc/spaceinvaders.h"

#include <QGraphicsScene>
#include <QTimer>

CSpaceInvaders::CSpaceInvaders(QSize oScreenSize, QWidget *pParent)
    : QGraphicsView(pParent), m_oScreenSize(oScreenSize) {
  QGraphicsScene *pScene = new QGraphicsScene();
  setScene(pScene);
  pScene->setSceneRect(0, 0, m_oScreenSize.width(), m_oScreenSize.height());

  setBackgroundBrush(QBrush(QImage(m_bg_image)));
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setCursor(Qt::PointingHandCursor);
  setStyleSheet(m_borderStyle);

  m_pSp_InvTimer = std::make_unique<QTimer>();
  connect(m_pSp_InvTimer.get(), &QTimer::timeout, this,
          &CSpaceInvaders::onLaunchEnemy);

  m_gameOver = false;
}

void CSpaceInvaders::Run() {
  scene()->clear();
  setCursor(Qt::BlankCursor);

  m_pCannon = std::make_unique<CCannon>(EColor::Red);

  m_pCannon->setPos(m_oScreenSize.width() / 2,
                    m_oScreenSize.height() - g_vars::gCannonSize.height());
  m_pCannon->setFlag(QGraphicsItem::ItemIsFocusable);
  m_pCannon->setFocus();

  scene()->addItem(m_pCannon.get());

  connect(m_pCannon.get(), &CCannon::sigIncreaseScore, this,
          &CSpaceInvaders::onIncreaseScore);
  connect(m_pCannon.get(), &CCannon::sigDecreaseScore, this,
          &CSpaceInvaders::onDecreaseScore);

  m_pPoints = std::make_unique<CPoints>();
  scene()->addItem(m_pPoints.get());

  // The function connected with timer(createEnemy) is called every 1.5 seconds
  startTimer(1200);
}

void CSpaceInvaders::CheckPoints() {

  if ((m_pPoints->GetScore() < 0) || (m_pPoints->GetHealth() <= 0)) {

    m_pPoints->Reset();
    onGameOver();
  }
}

void CSpaceInvaders::keyPressEvent(QKeyEvent *pEvent) {

  if (m_pCannon == nullptr)
    return;

  switch (pEvent->key()) {

  case Qt::Key_Left:
    if (m_pCannon->pos().x() > 0)
      m_pCannon->setPos(m_pCannon->x() - 60, m_pCannon->y());

    break;

  case Qt::Key_Right:
    if ((m_pCannon->pos().x() + g_vars::gCannonSize.width()) <
        m_oScreenSize.width())
      m_pCannon->setPos(m_pCannon->x() + 60, m_pCannon->y());

    break;

  case Qt::Key_V: {
    int nColor = static_cast<int>(m_pCannon->GetColor());
    m_pCannon->SetColor(static_cast<EColor>(++nColor));
  } break;

  case Qt::Key_Space:

    m_pCannon->Shoot();

    break;

  case Qt::Key_Q:
    if (m_gameOver) {
      close();
    }
    break;

  default:
    break;
  }
}

void CSpaceInvaders::onIncreaseScore() {
  m_pPoints->IncreaseScore();
  CheckPoints();
}

void CSpaceInvaders::onDecreaseScore() {

  m_pPoints->DecreaseScore();
  CheckPoints();
}

void CSpaceInvaders::onDecreaseHealth() {

  m_pPoints->DecreaseHealth();
  CheckPoints();
}

void CSpaceInvaders::onGameOver() {

  QList<QGraphicsItem *> allItems = scene()->items();
  for (auto item : allItems)
    if (dynamic_cast<CAlien *>(item))
      delete item;

  this->stopTimer();

  setBackgroundBrush(QBrush(QImage(m_gameOver_image)));
  m_gameOver = true;
}

void CSpaceInvaders::startTimer(uint16_t milliseconds) {
  m_pSp_InvTimer->start(milliseconds);
}

void CSpaceInvaders::stopTimer() { m_pSp_InvTimer->stop(); }

void CSpaceInvaders::onLaunchEnemy() {

  srand(time(NULL));
  int nPos = (rand() % m_oScreenSize.width()) - g_vars::gCannonSize.width();
  if (nPos < 0)
    nPos = 0;

  srand(time(NULL));

  int nColor = rand() % 3;

  m_pAlien = std::make_unique<CAlien>(static_cast<EColor>(nColor));
  m_pAlien->setPos(nPos, 0);

  connect(m_pAlien.get(), &CAlien::sigGameOver, this,
          &CSpaceInvaders::onGameOver);
  connect(m_pAlien.get(), &CAlien::sigDecreaseHealth, this,
          &CSpaceInvaders::onDecreaseHealth);

  scene()->addItem(m_pAlien.release());
}

CSpaceInvaders::~CSpaceInvaders() {

  if (m_pSp_InvTimer->isActive()) {
    stopTimer();
  }

  m_pSp_InvTimer.reset();
}


#include <QGraphicsView>
#include <QKeyEvent>
#include <QPointer>
#include <QTimer>

#include "components.h"

class CSpaceInvaders : public QGraphicsView {

  Q_OBJECT;

public:
  CSpaceInvaders(QSize oScreenSize, QWidget *pParent = nullptr);
  ~CSpaceInvaders();

  void Run();

  void CheckPoints();

protected:
  void keyPressEvent(QKeyEvent *pEvent) override;

public slots:
  void onLaunchEnemy();
  void onIncreaseScore();
  void onDecreaseScore();
  void onDecreaseHealth();
  void onGameOver();

private:
  std::unique_ptr<CCannon> m_pCannon;
  std::unique_ptr<CPoints> m_pPoints;
  std::unique_ptr<QTimer> m_pSp_InvTimer;
  std::unique_ptr<CAlien> m_pAlien;

  bool m_gameOver;

  QSize m_oScreenSize;
  QString m_bg_image = g_vars::gParFolderPath + "SpaceInvadersBg.jpg";
  QString m_gameOver_image = g_vars::gParFolderPath + "gameOver.png";
  QString m_borderStyle = "border-style:none";

  void startTimer(uint16_t milliseconds);
  void stopTimer();
};

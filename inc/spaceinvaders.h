
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPointer>
#include <QTimer>

#include "components.h"

class CSpaceInvaders : public QGraphicsView {

  Q_OBJECT;

public:
  CSpaceInvaders(QSize oScreenSize, QWidget *pParent = nullptr);

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
  CCannon *m_pCannon = nullptr;
  CPoints *m_pPoints = nullptr;
  QSize m_oScreenSize;
  QString m_bg_image = g_vars::gParFolderPath + "SpaceInvadersBg.jpg";
  QString m_gameOver_image = g_vars::gParFolderPath + "gameOver.png";
  QString m_borderStyle = "border-style:none";

  QTimer *m_pTimer = nullptr;

  void startTimer(uint16_t milliseconds);
  void stopTimer();

  ~CSpaceInvaders();
};

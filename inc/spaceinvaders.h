
#include <QPointer>
#include <QGraphicsView>
#include <QKeyEvent>

#include "components.h"

class CSpaceInvaders : public QGraphicsView
{

    Q_OBJECT;

public:
    CSpaceInvaders(QSize oScreenSize, QWidget *pParent = nullptr);

    void Run();

    void CheckPoints();

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;

public slots:
    void onCreateEnemy();
    void onIncreaseScore();
    void onDecreaseScore();
    void onDecreaseHealth();
    void onGameOver();

private:
    CCannon *m_pCannon = nullptr;
    CPoints *m_pPoints = nullptr;
    QSize m_oScreenSize;
    QString m_bg_image = "../Resources/SpaceInvadersBg.jpg";
    QString m_borderStyle = "border-style:none";
};

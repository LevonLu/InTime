#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include "config.h"
#include <QLabel>

namespace Ui {
class Popup;
}

class Popup : public QWidget
{
    Q_OBJECT

public:
    explicit Popup(QWidget *parent = nullptr);
    ~Popup();

private slots:
    void on_Popup_customContextMenuRequested(const QPoint &pos);
    void OnClickedPopMenu();
private:
    Ui::Popup *ui;
    Config *config;
    QTimer *timer;
    QLabel *mLbPrompt;
    QLabel *mLbTime;
    QColor *mColorNormal;
    QColor *mColorAlert;
    QColor *mColorText;
    int work_time;
    int left_time;
    void update();
protected:
    //拖拽窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool        m_bDrag;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;
};



#endif // POPUP_H

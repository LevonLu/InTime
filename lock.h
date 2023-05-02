#ifndef LOCK_H
#define LOCK_H

#include <QWidget>
#include "config.h"
#include <QLabel>


namespace Ui {
class lock;
}

class Lock : public QWidget
{
    Q_OBJECT

public:
    explicit Lock(QWidget *parent = nullptr);
    ~Lock();

    // 仅在需要显示文字时使用
    void update(int time);
    void initLabel(QRect rc, QString clr, QString prompt);
    bool isReset();

private:
    bool bShowText; // 是否显示文字
    bool bReset;    // 重置
    QLabel *mLbPrompt;
    QLabel *mLbTime;

protected:
    void keyPressEvent(QKeyEvent *event);
    //void keyReleaseEvent(QKeyEvent *event);
};


#endif // LOCK_H

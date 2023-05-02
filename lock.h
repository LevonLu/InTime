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

private:
    bool bShowText; // 是否显示文字
    QLabel *mLbPrompt;
    QLabel *mLbTime;
};


#endif // LOCK_H

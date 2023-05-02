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

private:
    Ui::lock *ui;
    Config *config;
    QTimer *timer;
    QLabel *mLbPrompt;
    QLabel *mLbTime;
    QColor *mColorNormal;
    int work_time;
    int relax_time;
    int left_time;
    void update();
    QString getPompt(int work);//应该独立出去
};


#endif // LOCK_H

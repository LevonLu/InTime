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
    void update(int time);
    void initLabel(QRect rc, QString clr, QString prompt);

private:
    //Ui::lock *ui;
    bool bShowText = false;
    QLabel *mLbPrompt;
    QLabel *mLbTime;
};


#endif // LOCK_H

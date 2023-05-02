
#ifndef LOCKMANAGER_H
#define LOCKMANAGER_H
#include "lock.h"
#include <QTimer>


class LockManager:public QObject
{
public:
    LockManager();
    LockManager(QList<QRect> lrect);
    void start();
    void stop();
    ~LockManager();

private:
    QList<Lock*> mListLock;
    QList<QRect> mListRect;
    void update();
    QTimer* mTimer;
    int work_time;
    int relax_time;
    int left_time;
    Config *mConfig;
};

QString getPompt(int work);

#endif // LOCKMANAGER_H

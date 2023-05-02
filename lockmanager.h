
#ifndef LOCKMANAGER_H
#define LOCKMANAGER_H
#include "lock.h"



class LockManager
{
public:
    LockManager();
    LockManager(QList<Lock*> llock, QList<QRect> lrect);
    void start();
    void stop();

private:
    QList<Lock*> mListLock;
    QList<QRect> mListRect;
};

#endif // LOCKMANAGER_H

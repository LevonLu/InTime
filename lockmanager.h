
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
    QList<Lock*> mListLock; // 为了适应多屏幕，使用Lock的list
    QList<QRect> mListRect; // 存储多个屏幕的尺寸和位置
    void update();          // 负责更新主屏幕上的文字信息
    QTimer* mTimer;
    int work_time;
    int relax_time;
    int left_time;
    Config *mConfig;
};

QString getPompt(int work);

#endif // LOCKMANAGER_H

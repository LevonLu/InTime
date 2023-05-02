
#include "lockmanager.h"
#include "popup.h"

LockManager::LockManager()
{

}

LockManager::LockManager(QList<QRect> lrect)
{
    mListRect = lrect;

    // 读取配置文件
    mConfig = new Config();
    QString strCNormal = mConfig->getValue({"Lock","color"}).toString();
    QString strCText = mConfig->getValue({"Lock","prompt","color"}).toString();
    double transparency = mConfig->getValue({"Lock","transparency"}).toDouble();
    work_time = mConfig->getValue({"Setting", "time", "work"}).toInt() * 5;
    relax_time = mConfig->getValue({"Setting", "time", "relax"}).toInt() * 5;
    left_time = relax_time;

    // 背景
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(strCNormal));

    // TODO 多屏幕锁屏会有延时
    foreach (QRect rc, mListRect){
        Lock* lock = new Lock();
        // 大小和位置
        lock->resize(rc.size());
        lock->move(rc.topLeft());
        // 去除标题栏 及任务栏图标 置顶
        lock->setWindowFlags( Qt::WindowFlags()&~Qt::Window | Qt::CustomizeWindowHint);
        lock->setWindowFlags( Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
        // 透明度
        lock->setWindowOpacity(transparency);
        // 背景
        lock->setAutoFillBackground(true);
        lock->setPalette(pal);
        //lock->show();
        mListLock.append(lock);
    }
    mListLock[0]->initLabel(mListRect[0],strCText,getPompt(work_time));
    mListLock[0]->update(left_time);
    for(int i = mListLock.size() - 1; i > 0; --i)
        mListLock[i]->show();



    mTimer = new QTimer(this);
    this->connect(mTimer, &QTimer::timeout, this, &LockManager::update);
    mTimer->start(1000);
}

LockManager::~LockManager()
{
    foreach (Lock* lock, mListLock){
        delete lock;
    }
    mListLock.clear();
}

void LockManager::update()
{
    if(left_time == 0)
    {
        mTimer->stop();
        delete this;
        Popup* popup = new Popup();
        popup->show();
        return;
    }
    mListLock[0]->update(left_time);
    left_time--;
    return;
}


QString getPompt(int work)
{
    QString strWorkTime = formatTime(work);
    QString prompt = QString("你已经工作了")+strWorkTime+QString("，请务必休息！站起来活动活动吧");
    return prompt;
}

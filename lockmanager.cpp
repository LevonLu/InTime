
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
    work_time = mConfig->getValue({"Setting", "time", "work"}).toInt() * 60;
    relax_time = mConfig->getValue({"Setting", "time", "relax"}).toInt() * 60;
    left_time = relax_time;

    // 背景
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(strCNormal));

    // 按照多显示器的信息来创建lock
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
        // 此处不统一调用show，是因为会出现主显示器上文字不出现的情况，原因暂不清楚。
        //lock->show();
        mListLock.append(lock);
    }

    // 给主显示器设置文字的初始化信息并做第一次更新
    mListLock[0]->initLabel(mListRect[0],strCText,getPompt(work_time));
    //mListLock[0]->update(left_time);

    // 显示其他显示器的lock
    for(int i = mListLock.size() - 1; i > 0; --i)
        mListLock[i]->show();

    // 更新计时器 启动！
    mTimer = new QTimer(this);
    this->connect(mTimer, &QTimer::timeout, this, &LockManager::update);
    mTimer->start(1000);

    mTimerReset = new QTimer(this);
    this->connect(mTimerReset, &QTimer::timeout, this, &LockManager::scanReset);
    mTimerReset->start(50);
}

LockManager::~LockManager()
{
    foreach (Lock* lock, mListLock){
        delete lock;
    }
    mListLock.clear();
    delete mTimer;
    delete mConfig;
}

void LockManager::update()
{
    if(left_time == 0)
    {
        mTimer->stop();
        mTimerReset->stop();
        delete this;
        Popup* popup = new Popup();
        popup->show();
        return;
    }
    mListLock[0]->update(left_time);
    left_time--;
    return;
}

void LockManager::scanReset()
{
    if(mListLock[0]->isReset())
    {
        mTimer->stop();
        mTimerReset->stop();
        delete this;
        Popup* popup = new Popup();
        popup->show();
        return;
    }
}


QString getPompt(int work)
{
    // TODO 读取配置文件来修改提示词
    QString strWorkTime = formatTime(work);
    QString prompt = QString("你已经工作了")+strWorkTime+QString("，请务必休息！站起来活动活动吧");
    return prompt;
}

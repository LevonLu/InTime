#include "lock.h"
#include "ui_lock.h"
#include <QTimer>
#include "popup.h"
#include <QScreen>

Lock::Lock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lock)
{
    ui->setupUi(this);
    mLbTime = this->findChild<QLabel*>("label_time");
    mLbPrompt = this->findChild<QLabel*>("label_prompt");

    // 去除标题栏 及任务栏图标 置顶
    setWindowFlags( windowFlags()&~Qt::Window | Qt::CustomizeWindowHint);
    setWindowFlags( Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);

    // 读取配置文件
    config = new Config();

    // 设置透明度及大小、位置
    setWindowOpacity(0.9);
    QList<QScreen*> screens = QGuiApplication::screens();
    QRect rc = screens[0]->geometry();
    resize(rc.size());
    move(rc.topLeft());

    QString strCNormal = config->getValue({"Lock","color"}).toString();
    QString strCText = config->getValue({"Popup","prompt","color"}).toString();
    mColorNormal = new QColor(strCNormal);


    // 更改背景颜色
    QPalette pal;
    pal.setColor(QPalette::Window, *mColorNormal);
    setAutoFillBackground(true);
    setPalette(pal);
    show();

    // 设置文字位置
    mLbPrompt->resize(rc.width(), 80);
    mLbTime->resize(rc.width(), 80);
    mLbPrompt->move(0, rc.height()/2 - 120);
    mLbTime->move(0, rc.height()/2 - 40 );

    // 设置文字颜色
    QString strTextStyleSheetColor = QString("color:")+strCText;
    mLbPrompt->setStyleSheet(strTextStyleSheetColor + "; font-size:20px");
    mLbTime->setStyleSheet(strTextStyleSheetColor + ";  font-size:60px");

    // 设置时间
    config = new Config();
    work_time = config->getValue({"Setting", "time", "work"}).toInt() * 60;
    relax_time = config->getValue({"Setting", "time", "relax"}).toInt() * 60;
    left_time = relax_time;
    mLbPrompt->setText(getPompt(work_time));
    //mLbTime->setText(formatTime(left_time));

    // 启动计时器 刷新时间
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Lock::update);
    timer->start(1000);

}

Lock::~Lock()
{
    delete ui;
    delete config;
    delete timer;
    delete mLbPrompt;
    delete mLbTime;
}

void Lock::update()
{
    if(left_time == 0)
    {
        timer->stop();
        this->destroy();
        Popup* popup = new Popup();
        popup->show();
    }
    QString strTime = formatTime(left_time);
    mLbTime->setText(strTime);
    left_time--;
    return;
}

QString Lock::getPompt(int work)
{
    QString strWorkTime = formatTime(work);
    QString prompt = QString("你已经工作了")+strWorkTime+QString("，请务必休息！站起来活动活动吧");
    return prompt;
}

#include "lock.h"
#include "config.h"

Lock::Lock(QWidget *parent) :
    QWidget(parent)
{
    bShowText = false;
    mLbTime = nullptr;
    mLbPrompt = nullptr;
}

Lock::~Lock()
{
    if(mLbPrompt)
        delete mLbPrompt;
    if(mLbTime)
        delete mLbTime;
}

// 更新第二行的倒计时，仅在调用过initLabel之后生效
void Lock::update(int time)
{
    if(bShowText)
    {
        QString strTime = formatTime(time);
        mLbTime->setText(strTime);
        show();
    }
}

// 初始化两个文字label的位置、大小、颜色 与第一行文字
void Lock::initLabel(QRect rc, QString clr, QString prompt)
{
    bShowText = true;
    mLbTime = new QLabel(this);
    mLbPrompt = new QLabel(this);

    mLbPrompt->resize(rc.width(), 80);
    mLbTime->resize(rc.width(), 80);
    mLbPrompt->move(rc.left(), rc.height()/2 - 120);
    mLbTime->move(rc.left(), rc.height()/2 - 40 );

    QString strTextStyleSheetColor = QString("color:")+clr;
    mLbPrompt->setStyleSheet(strTextStyleSheetColor + "; font-size:20px");
    mLbTime->setStyleSheet(strTextStyleSheetColor + ";  font-size:60px");

    mLbPrompt->setAlignment(Qt::AlignCenter);
    mLbTime->setAlignment(Qt::AlignCenter);

    mLbPrompt->setText(prompt);
}

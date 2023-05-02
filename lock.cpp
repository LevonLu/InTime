#include "lock.h"
//#include "ui_lock.h"
#include "config.h"

Lock::Lock(QWidget *parent) :
    QWidget(parent)//,
    //ui(new Ui::lock)
{
    mLbTime = nullptr;
    mLbPrompt = nullptr;
}

Lock::~Lock()
{
    //delete ui;
    if(mLbPrompt)
        delete mLbPrompt;
    if(mLbTime)
        delete mLbTime;
}

void Lock::update(int time)
{
    if(bShowText)
    {
        QString strTime = formatTime(time);
        mLbTime->setText(strTime);
        show();
    }
}

void Lock::initLabel(QRect rc, QString clr, QString prompt)
{
    bShowText = true;
//    mLbTime = this->findChild<QLabel*>("label_time");
//    mLbPrompt = this->findChild<QLabel*>("label_prompt");
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

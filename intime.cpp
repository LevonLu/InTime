
#include "intime.h"
#include "./ui_intime.h"
#include "popup.h"

InTime::InTime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InTime)
{
    ui->setupUi(this);
    setWindowFlags( windowFlags()&~Qt::WindowMinMaxButtonsHint | Qt::CustomizeWindowHint);
    setWindowIcon(QIcon(":/img/intime.png"));

    // 从配置文件读取工作和休息时间到编辑框
    config = new Config();
    mLnEditWork = this->findChild<QLineEdit*>("lineEdit_work");
    mLnEditRelax = this->findChild<QLineEdit*>("lineEdit_relax");
    auto work_time = config->getValue({"Setting", "time", "work"}).toInt();
    auto relax_time = config->getValue({"Setting", "time", "relax"}).toInt();
    mLnEditWork->setText(QString::number(work_time));
    mLnEditRelax->setText(QString::number(relax_time));
}


InTime::~InTime()
{
    delete config;
    delete mLnEditWork;
    delete mLnEditRelax;
    delete ui;
}


void InTime::on_pushButton_start_clicked()
{
    // 从编辑框保存时间到配置文件
    auto work_time = mLnEditWork->text();
    auto relax_time = mLnEditRelax->text();
    config->setValue(QJsonValue(work_time.toInt()), {"Setting", "time", "work"});
    config->setValue(QJsonValue(relax_time.toInt()), {"Setting", "time", "relax"});
    config->save();

    // 启动计时浮窗
    Popup* popup = new Popup();
    popup->show();
    this->destroy();
}



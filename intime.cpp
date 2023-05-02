
#include "intime.h"
#include "./ui_intime.h"
#include "popup.h"
#include "component.h"

InTime::InTime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InTime)
{
    ui->setupUi(this);
    setWindowFlags( windowFlags()&~Qt::WindowMinMaxButtonsHint | Qt::CustomizeWindowHint);
    setWindowIcon(QIcon(":/img/intime.png"));
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
    //Popup popup(this);
    auto work_time = mLnEditWork->text();
    auto relax_time = mLnEditRelax->text();
    config->setValue(QJsonValue(work_time.toInt()), {"Setting", "time", "work"});
    config->setValue(QJsonValue(relax_time.toInt()), {"Setting", "time", "relax"});
    config->save();
    Popup* popup = new Popup();
    popup->show();
    this->destroy();
}



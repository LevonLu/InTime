
#ifndef INTIME_H
#define INTIME_H

#include <QWidget>
#include "config.h"
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class InTime; }
QT_END_NAMESPACE

class InTime : public QWidget

{
    Q_OBJECT

public:
    InTime(QWidget *parent = nullptr);
    ~InTime();

private slots:
    void on_pushButton_start_clicked();

private:
    Ui::InTime *ui;
    Config *config;
    QLineEdit *mLnEditWork;
    QLineEdit *mLnEditRelax;

};

#endif // INTIME_H

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <initializer_list>

#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_PATH "D:/CODE/QT/InTime/intime.json"


class Config
{
public:
    Config();
    Config(QString p);

    void set_path(QString p);
    QString get_path();


    void load();
    void save();

    QString path;
    QJsonObject data;

    QJsonValue getValue(std::initializer_list<QString> il);
    //QJsonObject setValue(std::initializer_list<QString> il,  QJsonObject obj,QJsonValue val);
    bool setValue(QJsonValue val, std::initializer_list<QString> il);
    bool setValue_1(QString key, QJsonValue val);
    bool setValue_2(QString key1, QString key2, QJsonValue val);
    bool setValue_3(QString key1, QString key2, QString key3, QJsonValue val);
};

QString getCurPath();


#endif // CONFIG_H

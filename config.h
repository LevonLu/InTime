#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <initializer_list>

#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:
    Config();           // 默认构造函数 配置文件从主程序所在目录读取
    Config(QString p);

    void set_path(QString p);
    QString get_path();

    void load();
    void save();

    QString path;
    QJsonObject data;

    // TODO WARNING 下面的get和set均未检测异常，请谨慎使用
    QJsonValue getValue(std::initializer_list<QString> il);
    bool setValue(QJsonValue val, std::initializer_list<QString> il);
    bool setValue_1(QString key, QJsonValue val);
    bool setValue_2(QString key1, QString key2, QJsonValue val);
    bool setValue_3(QString key1, QString key2, QString key3, QJsonValue val);
};

QString getCurPath();
QString formatTime(int seconds);

#endif // CONFIG_H

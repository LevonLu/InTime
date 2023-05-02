#include "config.h"
#include <QFile>
#include <QApplication>

Config::Config()
{
    QString curPath = getCurPath();
    set_path(curPath+QString("/intime.json"));
    load();
}

Config::Config(QString p)
{
    set_path(p);
    load();
}

void Config::set_path(QString p)
{
    this->path = p;
}

QString Config::get_path()
{
    return this->path;
}

void Config::load()
{
    QString jsonFile = this->path;
    QFile myFile( jsonFile );
    if ( !myFile.open( QFile::ReadOnly ) )
    {
        qWarning("couldn't open json file.");
    }
    QByteArray data = myFile.readAll();
    myFile.close();
    QJsonParseError err;
    QJsonDocument json_doc = QJsonDocument::fromJson( data, & err );
    if ( json_doc.isNull() )
    {
        qDebug() << err.errorString();
    }
    this->data = json_doc.object();
}




void Config::save()
{
    // 打开文件
    QFile file(this->path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    // 将 QJsonDocument 对象转换为 JSON 字符串
    QByteArray content = QJsonDocument(this->data).toJson(QJsonDocument::Indented);

    // 写入 JSON 字符串到文件中
    file.write(content);
    file.close();
}

QJsonValue Config::getValue(std::initializer_list<QString> il)
{
    QJsonValue result;
    QJsonObject obj = this->data;
    for(auto arg:il)
    {
        result = obj[arg];
        obj = result.toObject();
    }
    return result;
}

bool Config::setValue(QJsonValue val, std::initializer_list<QString> il)
{
    int len = il.size();
    switch(len)
    {
    case 1:setValue_1(*(il.begin()), val);break;
    case 2:setValue_2(*(il.begin()), *(il.begin()+1), val);break;
    case 3:setValue_3(*(il.begin()), *(il.begin()+1), *(il.begin()+2) ,val);break;
    default:
        return false;
    }
    return true;
}

bool Config::setValue_1(QString key, QJsonValue val)
{
    data[key] = val;
    return true;
}

bool Config::setValue_2(QString key1, QString key2, QJsonValue val)
{
    QJsonObject obj = data[key1].toObject();
    obj[key2] = val;
    data.remove(key1);
    data.insert(key1, obj);
    return true;
}

bool Config::setValue_3(QString key1, QString key2, QString key3, QJsonValue val)
{
    QJsonObject obj1 = data[key1].toObject();
    QJsonObject obj2 = obj1[key2].toObject();
    obj2[key3] = val;

    obj1.remove(key2);
    obj1.insert(key2, obj2);

    data.remove(key1);
    data.insert(key1, obj1);
    return true;
}



QString formatTime(int seconds)
{
    // TODO 支持更长时间
    // 计算分钟和秒钟数
    int minutes = seconds / 60;
    seconds %= 60;

    // 使用 sprintf() 函数将数字格式化成 "xx:xx" 格式的字符串
    char str[6];
    sprintf(str, "%02d:%02d", minutes, seconds);

    // 将字符数组转换为 QString 类型
    return QString(str);
}


QString getCurPath()
{
    QDir dir;
    return dir.absolutePath();
}

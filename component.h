#include <QString>
#include <string>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

#ifndef COMPONENT_H
#define COMPONENT_H


class component
{
public:
    component();
};

QJsonDocument read_json(QString file_path);

#endif // COMPONENT_H

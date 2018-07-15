#include "utils.h"
#include "appconfig.h"

#include <QObject>
#include <QFile>
#include <QList>
#include <QStringList>

#include <QDebug>

bool writeLineToFile(QString f, QString data)
{
    QFile file(f);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString() << f;
        return false;
    }

    data += "\n";
    file.write(data.toUtf8());
    file.close();

    return true;
}


QString readLineFromFile(QString f)
{
    QString l;
    QFile file(f);
    if (!file.open(QIODevice::ReadOnly)) {
        //qDebug() << file.errorString() << f;
        return l;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        l = QString::fromLocal8Bit(line);
    }
    file.close();

    return l;
}

bool readCSVFile(QString f, QList<QStringList> &l)
{
    QFile file(f);
    if (!file.open(QIODevice::ReadOnly)) {
        //qDebug() << file.errorString() << f;
        return false;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        QString lline = QString::fromLocal8Bit(line);
        l.append(lline.split(';'));
    }
    file.close();

    return true;
}

#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QList>
#include <QStringList>

QString readLineFromFile(QString f);
bool readCSVFile(QString f, QList<QStringList> &l);
bool writeLineToFile(QString f, QString data);

QString settingsRootPath();
QString settingsPath();

QString inputsRootPath();
QString outputRootPath();

bool isSimMode();
QString simRootPath();
QString simPath();
int simTick();
QString getSimTimeClock();

#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include <QString>

QString readLineFromFile(QString f);

QString settingsRootPath();
QString settingsPath();

QString inputsRootPath();

bool isSimMode();
QString simRootPath();
QString simPath();
int simTick();
QString getSimTimeClock();

#endif // UTILS_H

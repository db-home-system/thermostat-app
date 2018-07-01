#ifndef UTILS_H
#define UTILS_H

#include <QString>

bool isSimMode();
QString settingsRootPath();
QString settingsPath();

QString simRootPath();
QString simPath();
int simTick();
QString getTimeClock();

#endif // UTILS_H

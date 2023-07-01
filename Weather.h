//
// Created by HarryXiong on 2023/7/1.
//

#ifndef FSD_REBASED_WEATHER_H
#define FSD_REBASED_WEATHER_H


#include <QString>
#include <QHash>
#include "Global.h"
#include <QObject>
class Weather :public QObject{
    Q_OBJECT
public:
    Weather();
    void UpdateWeather();
    QString getMetar(QString icao);
private:
    QHash<QString,QString> metarList;
};


#endif //FSD_REBASED_WEATHER_H

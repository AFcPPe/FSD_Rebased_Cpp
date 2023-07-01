//
// Created by HarryXiong on 2023/6/29.
//

#ifndef FSD_REBASED_REDIS_H
#define FSD_REBASED_REDIS_H

#include <QObject>
#include "sw/redis++/redis++.h"
//using namespace sw::redis;
class Redis: public QObject{
    Q_OBJECT
public:
    Redis();
    bool setHashValue(int db,QString key,QString field, QString value,int expire);
    QString getHashValue(int db,QString key,QString field);
private:
    sw::redis::ConnectionOptions option;
private slots:

};


#endif //FSD_REBASED_REDIS_H

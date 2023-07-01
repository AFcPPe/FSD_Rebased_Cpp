//
// Created by HarryXiong on 2023/6/29.
//

#include "Redis.h"
#include "Global.h"
//using namespace sw::redis;
Redis::Redis() {
    option.host = Global::get().s.redisSettings.address.toStdString();
    option.port = Global::get().s.redisSettings.port;
    option.password = Global::get().s.redisSettings.password.toStdString();
    option.db = 15;
    auto redis = sw::redis::Redis(option);
    if(QString(redis.get("ServerStatus")->c_str()) != "Good"){
        qFatal()<<"Failed to connect to Redis. Shutting down the server. Please check network and redis settings.";
        exit(1002);
    }
    qInfo()<<"Connected to Redis.";
}

bool Redis::setHashValue(int db,QString key, QString field, QString value,int expire) {
    try{
        sw::redis::ConnectionOptions opt = sw::redis::ConnectionOptions(option);
        opt.db = db;
        auto redis = sw::redis::Redis(option);
        redis.hset(key.toStdString(),field.toStdString(),value.toStdString());
        redis.expire(key.toStdString(),expire);
        return true;
    }catch (...){
        return false;
    }

}

QString Redis::getHashValue(int db, QString key, QString field) {
    try{
        sw::redis::ConnectionOptions opt = sw::redis::ConnectionOptions(option);
        opt.db = db;
        auto redis = sw::redis::Redis(option);
        return redis.hget(key.toStdString(),field.toStdString())->c_str();
    }catch (...){
        return{};
    }

}

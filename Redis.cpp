//
// Created by HarryXiong on 2023/6/29.
//

#include "Redis.h"
#include "Global.h"

Redis::Redis() {
    c = (redisContext*)redisConnectWithTimeout(Global::get().s.redisSettings.address.toStdString().c_str(), Global::get().s.redisSettings.port,{1, 500000});
    if (c == nullptr || c->err) {
        qFatal()<<"Failed to connect to Redis. Shutting down the server. Please check network and redis settings.";
        exit(1002);
    } else {
        qInfo()<<"Connected to Redis.";
        qInfo()<<"Authenticating to Redis server.";
        auto reply = (redisReply*)redisCommand(c,"AUTH %s",Global::get().s.redisSettings.password.toStdString().c_str());
        if(reply == NULL||reply->type == REDIS_REPLY_ERROR){
            qInfo()<<"Failed to Authenticate to Redis Server. Shutting down the server.";
            exit(1003);
        }
        qInfo()<<"Authentication to Redis server is successful.";
    }
}

bool Redis::setHashValue(int db,QString key, QString field, QString value,int expire) {
    auto reply = (redisReply*)redisCommand(c,QString("SELECT %1").arg(db).toStdString().c_str());
    if(reply->type == REDIS_REPLY_ERROR){
        return false;
    }
    reply = (redisReply*)redisCommand(c,QString("HSET %1 %2 %s").arg(key,field).toStdString().c_str(),value.toStdString().c_str());
    if(reply->type == REDIS_REPLY_ERROR){
        return false;
    }
    reply = (redisReply*)redisCommand(c,QString("expire %1 %2").arg(key).arg(expire).toStdString().c_str());
    if(reply->type == REDIS_REPLY_ERROR){
        return false;
    }
    return true;
}

QString Redis::getHashValue(int db, QString key, QString field) {
    auto reply = (redisReply*)redisCommand(c,QString("SELECT %1").arg(db).toStdString().c_str());
    if(reply->type == REDIS_REPLY_ERROR){
        return "";
    }
    reply = (redisReply*)redisCommand(c,QString("HGET %1 %2").arg(key).arg(key,field).toStdString().c_str());
    if(reply->type == REDIS_REPLY_ERROR){
        return "";
    }
    return {reply->str};
}

//
// Created by HarryXiong on 2023/6/24.
//


#include "Mysql.h"

Mysql::Mysql() {
    db_user = QSqlDatabase::addDatabase("QMYSQL");
    db_user.setHostName(Global::get().s.mysqlSettings.address);
    db_user.setPort(Global::get().s.mysqlSettings.port);
    db_user.setDatabaseName(Global::get().s.mysqlSettings.db_user);
    db_user.setUserName(Global::get().s.mysqlSettings.account);
    db_user.setPassword(Global::get().s.mysqlSettings.password);
    if(!db_user.open()){
        qFatal()<<"Failed to connect to Mysql. Shutting down the server. Please check network and mysql settings.";
        exit(1001);
    }else{
        qInfo()<<"Connected to Mysql";
    }
}

QSqlQuery Mysql::queryUser(QString sql) {
    if(!db_user.open()) return QSqlQuery();
    QSqlQuery query(db_user);
    query.exec(sql);
    db_user.close();
    return query;
}

UserInfo Mysql::getUserInfo(QString cid) {
    QSqlQuery result = queryUser(QString("SELECT username,password,level from newuser where username=%1 limit 1").arg(cid));
    result.next();
    if(result.isValid()){
        return UserInfo(result.value(0).toString(), result.value(1).toString(),
                        static_cast<NetworkRating>(result.value(2).toInt()));
    }
    return {};
}

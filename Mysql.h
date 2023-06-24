//
// Created by HarryXiong on 2023/6/24.
//

#ifndef FSD_REBASED_MYSQL_H
#define FSD_REBASED_MYSQL_H

#include <QtSql/qsqldatabase.h>
#include "Global.h"
#include "enums.h"
#include <QSqlQuery>
enum MysqlStatus{
    mConnected,
    mDisconnected
};

struct UserInfo{
    QString cid;
    QString encryptedPassword;
    NetworkRating rating;
};

class Mysql: public QObject{
    Q_OBJECT
public:
    Mysql();
    //以下是外部调用部分
    UserInfo getUserInfo(QString cid);
private:
    QSqlDatabase db_user;
    MysqlStatus status;
    QSqlQuery queryUser(QString sql);
private slots:

};


#endif //FSD_REBASED_MYSQL_H

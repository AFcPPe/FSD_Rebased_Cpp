//
// Created by HarryXiong on 2023/6/23.
//

#ifndef FSD_REBASED_SETTINGS_H
#define FSD_REBASED_SETTINGS_H


#include <QString>
#include <QList>
#include <QDebug>

struct MysqlSettings{
    QString address;
    int port;
    QString account;
    QString password;
    QString db_user;
    QString table_user;
    QString db_data;
};

class Settings {
public:
    QList<QString> qlsMotd;
    unsigned short usServerPort = 6809;
    MysqlSettings mysqlSettings;
    void load();
    Settings();
};


#endif //FSD_REBASED_SETTINGS_H

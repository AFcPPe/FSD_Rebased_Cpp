//
// Created by HarryXiong on 2023/6/23.
//

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Settings.h"

void Settings::load() {
    QFile file("./settings.json");
    QByteArray jsonData;
    if(file.open(QIODevice::ReadOnly)){
        jsonData = file.readAll();
        file.close();
    }
    QJsonDocument jsonDocu = QJsonDocument::fromJson(jsonData);
    if(jsonDocu.isObject()){
        QJsonObject obj_root = jsonDocu.object();
        //读取motd
        auto motdArray =  obj_root.value("qlsMotd").toArray();
        for(auto motd : motdArray){
            this->qlsMotd.push_back(motd.toString());
        }
        //读取服务器端口
        this->usServerPort =  obj_root.value("server port").toInteger();
        //读取数据库信息
        auto obj_mysql = obj_root.value("mysql").toObject();
        this->mysqlSettings.address = obj_mysql.value("address").toString();
        this->mysqlSettings.port = obj_mysql.value("port").toInt();
        this->mysqlSettings.db_user = obj_mysql.value("db_user").toString();
        this->mysqlSettings.db_data = obj_mysql.value("db_data").toString();
        this->mysqlSettings.table_user = obj_mysql.value("table_user").toString();
        this->mysqlSettings.account = obj_mysql.value("account").toString();
        this->mysqlSettings.password = obj_mysql.value("password").toString();
        //读取服务器相关设置
        this->status_check_time = obj_root.value("status_check_time").toInt();
    }

}

Settings::Settings() {
    this->qlsMotd = QList<QString>();
}

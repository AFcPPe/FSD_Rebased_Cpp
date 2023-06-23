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
    }

}

Settings::Settings() {
    this->qlsMotd = QList<QString>();
}

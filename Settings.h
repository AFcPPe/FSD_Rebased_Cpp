//
// Created by HarryXiong on 2023/6/23.
//

#ifndef FSD_REBASED_SETTINGS_H
#define FSD_REBASED_SETTINGS_H


#include <QString>
#include <QList>
#include <QDebug>

class Settings {
public:
    QList<QString> qlsMotd;
    unsigned short usServerPort = 6809;
    void load();
    Settings();
};


#endif //FSD_REBASED_SETTINGS_H

//
// Created by HarryXiong on 2023/7/1.
//

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "Weather.h"


QString Weather::getMetar(QString ICAO) {
    if(metarList.contains(ICAO)){
        return metarList.value(ICAO);
    }
    return {};
}

Weather::Weather() {

}

void Weather::UpdateWeather() {
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager();
    QString url = Global::get().s.MetarRefreshUrl;
    QNetworkRequest req((QUrl(url)));
    QNetworkReply *reply =  m_pHttpMgr->get(req);
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError){
        QString data = reply->readAll();
        auto metarL = data.split("\n");
        metarList.clear();
        for (auto me:metarL){
            metarList.insert(me.split(" ")[0],me);
        }
    }
}

//
// Created by HarryXiong on 2023/6/23.
//



#ifndef FSD_REBASED_GLOBAL_H
#define FSD_REBASED_GLOBAL_H

#include <QtCore>
#include "Settings.h"
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137

class Server;
class Mysql;

class Global{
public:
    static Global *g_global_struct;
    static Global &get();
    Settings s;
    Server *server;
    Mysql *mysql;
};


#endif //FSD_REBASED_GLOBAL_H

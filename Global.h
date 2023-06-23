//
// Created by HarryXiong on 2023/6/23.
//



#ifndef FSD_REBASED_GLOBAL_H
#define FSD_REBASED_GLOBAL_H

#include <QtCore>
#include "Settings.h"

class Server;

class Global{
public:
    static Global *g_global_struct;
    static Global &get();
    Settings s;
    Server *server;
};


#endif //FSD_REBASED_GLOBAL_H

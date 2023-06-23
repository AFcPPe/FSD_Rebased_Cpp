//
// Created by HarryXiong on 2023/6/23.
//

#include "Global.h"

Global *Global::g_global_struct;

Global &Global::get() {
    return *g_global_struct;
}
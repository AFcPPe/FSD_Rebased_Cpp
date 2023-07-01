#include <QCoreApplication>
#include <QDebug>
#include "Global.h"
#include "Server.h"
#include "Mysql.h"
#include "Weather.h"

void log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
        case QtDebugMsg:
#ifndef NDEBUG
            text = QString("DEBUG");
            break;
#else
            return;
#endif

        case QtWarningMsg:
            text = QString("WARNING");
            break;
        case QtCriticalMsg:
            text = QString("CRITICAL");
            break;
        case QtFatalMsg:
            text = QString("FATAL");
            break;
        case QtInfoMsg:
            text = QString("INFO");
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("%1").arg(current_date_time);
    QString message = QString("[%2] [%1]: %3").arg(text,current_date, msg);
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    qDebug()<<qPrintable(message);
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    //注册日志系统
    qInstallMessageHandler(log);
    qInfo()<<"SKYline FSD starting";
    qInfo()<<"Loading Settings";
    //创建全局对象
    Global::g_global_struct = new Global();
    //读取设置
    Global::get().s.load();
    qInfo()<<"Settings have been loaded";
    //连接Mysql服务器
    qInfo()<<"Connecting to Mysql Server";
    Global::get().mysql = new Mysql();
    //创建Weather对象
    qInfo()<<"Creating Weather Manager";
    Global::get().weather = new Weather();
    Global::get().weather->UpdateWeather();
    //创建服务器
    qInfo()<<"Initializing Server.";
    Global::get().server = new Server();
    return QCoreApplication::exec();
}

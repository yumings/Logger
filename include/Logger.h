#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "log_define.h"
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif 
#define DllExport   __declspec(dllexport)
/*
    * 类名：Logger
    * 作用：提供写日志功能，支持多线程，支持可变形参数操作，支持写日志级别的设置
    * 接口：SetLogLevel：设置写日志级别
            TraceInfo：写信息
*/
#ifdef _WIN32
class DllExport Logger
#else
class Logger
#endif
{

private:

    Logger(std::string path);//私有构造函数
    Logger(const Logger& log){}//禁止拷贝 
	Logger& operator=(const Logger&)
    {
        return *this;
    }//禁止赋值
#ifdef _WIN32
    static HANDLE mutex;    //互斥锁
#else
	static pthread_mutex_t mutex;
#endif
    static Logger* instance;
public:
	static Logger* getInstance(std::string path);

    virtual ~Logger();
public:
	//写入信息 ,默认只记录错误信息
    void TraceInfo(LogLevel logLevel,std::string strInfo);

private:
    std::ofstream g_fileStream;//文件流
    LogLevel nLogLevel = LogLevel::Log_Error;// logLevel 级别
    std::string logFilePath;//log 路径
    std::string logFileName;//log文件名称
private:
    //写文件操作
    void createLogPath();
    void createLogFilename(); //创建log文件
    std::string GetCurrentSystemTime();//获取当前系统时间 年，月，日
    std::string GetCurrTime();//时，分，秒
    void Trace(std::string& logInfo);//写入跟踪信息  

};


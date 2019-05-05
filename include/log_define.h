#pragma once
#ifndef LOG_DEFINE_H
#define LOG_DEFINE_H

//日志级别的提示信息 

typedef enum LogLevel
{
    Log_debug = 1,
    Log_Info = 2,// 记录全部信息
    Log_Warning =4 , // 记录警告信息
    Log_Error = 8, //只记录错误信息
}LogLevel;
#endif
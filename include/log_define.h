#pragma once
#ifndef LOG_DEFINE_H
#define LOG_DEFINE_H

//日志级别的提示信息 

typedef enum LogLevel
{
    Log_Info = 0,// 记录全部信息
    Log_Warning , // 记录警告信息和错误信息
    Log_Error, //只记录错误信息
}LogLevel;
#endif
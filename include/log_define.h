#pragma once
#ifndef LOG_DEFINE_H
#define LOG_DEFINE_H

//��־�������ʾ��Ϣ 
static const int MAX_STR_LEN = 1024;

typedef enum LogLevel
{
    Log_Info = 0,// ��¼ȫ����Ϣ
    Log_Warning , // ��¼������Ϣ�ʹ�����Ϣ
    Log_Error, //ֻ��¼������Ϣ
}LogLevel;
#endif
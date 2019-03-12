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
#define DllExport   __declspec( dllexport )
/*
    * ������Logger
    * ���ã��ṩд��־���ܣ�֧�ֶ��̣߳�֧�ֿɱ��β���������֧��д��־���������
    * �ӿڣ�SetLogLevel������д��־����
            TraceInfo��д��Ϣ
*/
#ifdef _WIN32
class DllExport Logger
#else
class Logger
#endif
{

private:

    Logger();//˽�й��캯��
    Logger(const Logger& log){}//��ֹ���� 
	Logger& operator=(const Logger&)
    {
        return *this;
    }//��ֹ��ֵ
#ifdef _WIN32
    static HANDLE mutex;    //������
#else
	static pthread_mutex_t mutex;
#endif
    static Logger* instance;
public:
	static Logger* getInstance();

    virtual ~Logger();
public:
	//д����Ϣ ,Ĭ��ֻ��¼������Ϣ
    void TraceInfo(LogLevel logLevel,std::string strInfo);
    void setLogPath(std::string& logPtah);


private:
    std::ofstream g_fileStream;//�ļ���
    LogLevel nLogLevel;// logLevel ����
    std::string logFilePath;//log ·��
    std::string logFileName;//log�ļ�����
private:
    //д�ļ�����
    void createLogPath();
    void createLogFilename(); //����log�ļ�
    std::string GetCurrentSystemTime();//��ȡ��ǰϵͳʱ�� �꣬�£���
    std::string GetCurrTime();//ʱ���֣���
    void Trace(std::string& logInfo);//д�������Ϣ  

};


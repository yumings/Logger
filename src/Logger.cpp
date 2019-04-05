#include "Logger.h"
#include <time.h>
#include <string>
#include <fstream>



//默认的构造函数，设置错误级别，记录错误信息
Logger::Logger()
{
    logFilePath = ".";//默认当前目录
    createLogPath();
    createLogFilename();
}


Logger* Logger::getInstance()
{	
	if(instance==nullptr){
#ifdef _WIN32
        WaitForSingleObject(mutex, INFINITE);
#else
		  pthread_mutex_lock(&mutex);
#endif
          if(instance==nullptr){//需要时创建对象
		  	Logger *log = new Logger(); 
			instance = log;
		  }
#ifdef _WIN32
          ReleaseMutex(mutex);
#else
		  pthread_mutex_unlock(&mutex);
#endif
	}
	return instance;
}
//互斥量初始化
#ifdef _WIN32
HANDLE Logger::mutex = nullptr;
#else

pthread_mutex_t Logger::mutex = PTHREAD_MUTEX_INITIALIZER;

#endif // _WIN32


Logger* Logger::instance= nullptr;//先初始化一个空值，然后需要对象时创建对象。饱汉模式

//释放资源
Logger::~Logger()
{
    //释放临界区 
    if (g_fileStream.is_open())
        g_fileStream.close();
}

//写入信息
void Logger::TraceInfo(LogLevel logLevel,std::string strInfo)
{
    if (strInfo.empty())
        return;
    std::string temp = "["+GetCurrTime()+"]";
	std::string prefix = "";
	if(logLevel == LogLevel::Log_Error)
		prefix = "ERRO";
	else if(logLevel == LogLevel::Log_Warning)
		prefix = "WARN";
	else if(logLevel == LogLevel::Log_Info)
		prefix = "INFO";
    temp += " ["+ prefix+"] \t" + strInfo;
    Trace(temp);
}

void MakeSureDirectoryPathExists(std::string & filePath)
{
#ifdef _WIN32
    if (0 != _access(filePath.c_str(), 0)) {
        filePath = "";
    }
#else
    if (0 != access(filePath.c_str(), 0)) {
        filePath = "";
    }
#endif
}

void Logger::createLogPath()
{
    if (logFilePath.back() != '/')
        logFilePath += "/";
    MakeSureDirectoryPathExists(logFilePath);
    logFileName = GetCurrentSystemTime();
}

void Logger::setLogPath(std::string& logPath)
{
    logFilePath = logPath;
    logFileName = GetCurrentSystemTime();
}

void Logger::createLogFilename()
{
    if (GetCurrentSystemTime().compare(logFileName) != 0) {
        logFileName = GetCurrentSystemTime();
    }
    std::string absFile = logFilePath + logFileName;
    std::ofstream(absFile.c_str(),std::ios::app);
}




void Logger::Trace(std::string & logInfo)
{
    if (logInfo.empty())
        return;
    try
    {
        //若文件流没有打开，则重新打开 
        if (!g_fileStream.is_open())
        {
            if(!logFilePath.empty())
                if (logFilePath.back() != '/')
                    logFilePath += "/";
            std::string temp = logFilePath + logFileName;
			std::cout << temp << std::endl;
            g_fileStream.open(temp,std::ios::app);
            if (!g_fileStream.is_open())
            {
            	std::cout<< "open failed"<<std::endl;
                return;
            }
        }
        //写日志信息到文件流 
        //std::cout<< logInfo << std::endl;
        g_fileStream << logInfo << std::endl;
        g_fileStream.flush();
 
    }
    catch (...)
    {
        
    }
}

std::string Logger::GetCurrentSystemTime()
{//获取系统时间
#ifdef _WIN32
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    char timeStr[20];
    sprintf_s(timeStr, "%4d-%02d-%02d.log", sys.wYear, sys.wMonth, sys.wDay);
    std::string logFileName(timeStr);
#else 
    struct timeval tv = {0};
    gettimeofday(&tv,NULL);   
    struct tm * pTimeInfo = localtime(&tv.tv_sec);
    char filename[20] = {0};
    sprintf(filename,"%.4d-%.2d-%.2d.log",pTimeInfo->tm_year + 1900,pTimeInfo->tm_mon + 1,pTimeInfo->tm_mday);
    std::string logFileName(filename);
#endif
    return logFileName;
}

std::string Logger::GetCurrTime()
{
#ifdef _WIN32
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    char timeStr[20];
    sprintf_s(timeStr,"%02d:%02d:%02d.%03ld", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
    std::string str(timeStr);
#else
    struct timeval tv = {0};
    gettimeofday(&tv,NULL);   
    struct tm * timeinfo = NULL;
    timeinfo = localtime(&tv.tv_sec);
    char timeStr[20] = {0};
    sprintf(timeStr, "%.2d:%.2d:%.2d.%03ld", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,tv.tv_usec/1000);
    std::string str(timeStr);
#endif
    return str;
}

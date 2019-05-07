#include "Logger.h"
#include <time.h>
#include <string>
#include <fstream>

//默认的构造函数，设置错误级别，记录错误信息
Logger::Logger(std::string path)
{
    if (path.empty())
        logFilePath = "."; //默认当前目录
    else
        logFilePath = path;
    getLogPathAndLogName();
}

//调试等级 writeLevel
Logger *Logger::getInstance(std::string path, int writeLevel)
{
    if (instance == nullptr)
    {
#ifdef _WIN32
        WaitForSingleObject(mutex, INFINITE);
#else
        pthread_mutex_lock(&mutex);
#endif
        if (instance == nullptr)
        { //需要时创建对象
            Logger *log = new Logger(path);
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

Logger *Logger::instance = nullptr; //先初始化一个空值，然后需要对象时创建对象。饱汉模式

//释放资源
Logger::~Logger()
{
    //释放临界区
    if (g_fileStream.is_open())
        g_fileStream.close();
}

//写入信息
void Logger::TraceInfo(LogLevel logLevel, std::string strInfo)
{
    if (strInfo.empty())
        return;
    std::string temp = "[" + GetCurrTime() + "]";
    std::string prefix = "";
    if (logLevel == LogLevel::Log_Error)
        prefix = "ERRO";
    else if (logLevel == LogLevel::Log_Warning)
        prefix = "WARN";
    else if (logLevel == LogLevel::Log_Info)
        prefix = "INFO";
    temp += " [" + prefix + "] \t" + strInfo;
    Trace(temp);
}

void MakeSureDirectoryPathExists(std::string &filePath)
{
#ifdef _WIN32
    if (0 != _access(filePath.c_str(), 0))
    { //如果文件
        filePath = "";
    }
#else
    if (0 != access(filePath.c_str(), 0))
    {
        filePath = "";
    }
#endif
}

void Logger::getLogPathAndLogName()
{
    if (logFilePath.back() != '/')
        logFilePath += "/";
    MakeSureDirectoryPathExists(logFilePath);
    if (GetCurrentSystemTime().compare(logFileName) != 0)
    {
        logFileName = GetCurrentSystemTime();
    }
}


void Logger::Trace(std::string &logInfo)
{
    if (logInfo.empty())
        return;
    try
    {
        //若文件流没有打开，则重新打开
        if (!g_fileStream.is_open())
        {
            if (!logFilePath.empty())
                if (logFilePath.back() != '/')
                    logFilePath += "/";
            std::string temp = logFilePath + logFileName;
            std::cout << temp << std::endl;
        

            g_fileStream.open(temp, std::ios::out|std::ios::app);
            if (!g_fileStream.is_open())
            {
                std::cout << "open failed" << std::endl;
                return;
            }
        }
        //写日志信息到文件流
        //处理log文件大小超过规定的大小的处理方法
        size_t logSize = GetLogFileSize();
        if(logSize > 1024*1024){ //如果文件的大小大于5M 则清理文件
            ClearLogFile();
        }
      
        std::streampos pos = g_fileStream.tellg();
        std::cout << pos << std::endl;
        g_fileStream << logInfo << std::endl;
        g_fileStream.flush();
    }
    catch (...)
    {
    }
}

std::string Logger::GetCurrentSystemTime()
{ //获取系统时间
#ifdef _WIN32
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    char timeStr[20];
    sprintf_s(timeStr, "%4d-%02d-%02d.log", sys.wYear, sys.wMonth, sys.wDay);
    std::string logFileName(timeStr);
#else
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    struct tm *pTimeInfo = localtime(&tv.tv_sec);
    char filename[20] = {0};
    sprintf(filename, "%.4d-%.2d-%.2d.log", pTimeInfo->tm_year + 1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
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
    sprintf_s(timeStr, "%02d:%02d:%02d.%03ld", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
    std::string str(timeStr);
#else
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    struct tm *timeinfo = NULL;
    timeinfo = localtime(&tv.tv_sec);
    char timeStr[20] = {0};
    sprintf(timeStr, "%.2d:%.2d:%.2d.%03ld", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tv.tv_usec / 1000);
    std::string str(timeStr);
#endif
    return str;
}

//获取log文件的大小
size_t Logger::GetLogFileSize()
{
    std::string tmp = logFilePath + logFileName;
    g_fileStream.open(tmp,std::ios::out | std::ios::app);
    std::streampos cur_pos = g_fileStream.tellg();//获取当前的指针指向的位置 
    g_fileStream.seekg(0,std::ios::end); //指针指向文件末尾
    std::streampos pos = g_fileStream.tellg();//获取文件的长度
    g_fileStream.seekg(cur_pos);//恢复指针指向的位置
    g_fileStream.close();
    return static_cast<size_t>(pos);
}


//关闭log文件
void Logger::CloseLogFile()
{
    g_fileStream.clear();  // 清理流状态
    g_fileStream.close();  // 关闭流

}

//清理log文件内容
void Logger::ClearLogFile()
{
    CloseLogFile(); // 关闭流
    std::string tmp = logFilePath + logFileName;
    g_fileStream.open(tmp, std::ios::out | std::ios::trunc); // 以截断方式打开文件
}
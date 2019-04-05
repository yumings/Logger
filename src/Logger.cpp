#include "Logger.h"
#include <time.h>
#include <string>
#include <fstream>



//Ĭ�ϵĹ��캯�������ô��󼶱𣬼�¼������Ϣ
Logger::Logger()
{
    nLogLevel = LogLevel::Log_Error;// logLevel ����	 
    logFilePath = ".";//Ĭ�ϵ�ǰĿ¼
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
          if(instance==nullptr){//��Ҫʱ��������
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
//��������ʼ��
#ifdef _WIN32
HANDLE Logger::mutex = nullptr;
#else

pthread_mutex_t Logger::mutex = PTHREAD_MUTEX_INITIALIZER;

#endif // _WIN32


Logger* Logger::instance= nullptr;//�ȳ�ʼ��һ����ֵ��Ȼ����Ҫ����ʱ�������󡣱���ģʽ

//�ͷ���Դ
Logger::~Logger()
{
    //�ͷ��ٽ��� 
    if (g_fileStream.is_open())
        g_fileStream.close();
}

//д����Ϣ
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
	//std::cout << temp << std::endl;
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
        //���ļ���û�д򿪣������´� 
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
        //д��־��Ϣ���ļ��� 
        //std::cout<< logInfo << std::endl;
        g_fileStream << logInfo << std::endl;
        g_fileStream.flush();
 
    }
    catch (...)
    {
        
    }
}

std::string Logger::GetCurrentSystemTime()
{//��ȡϵͳʱ��
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

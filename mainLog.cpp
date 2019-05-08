#include <Logger.h>
#include <log_define.h>
#include <thread>
#include <string>
#include "cjson.h"

//
void fun(Logger* log,std::string& a)
{
    for (int i = 0; i < 10000; ++i)
    {
        std::string info = "thread:" + a;   
        log->TraceInfo(LogLevel::Log_Info,info);
		Sleep(1);
    }
}

int checkJsonFormat(const rapidjson::Document& doc)
{

	return 0;
}

int main(int argc,char* argv[])
{
	//读取配置文件
	// std::string  jsonFileName = "config.json";
	// rapidjson::Document doc = nullptr;
	// int ret = GetDocumentFromJsonFile(jsonFileName, doc);
	// if (ret < 0) {
	// 	std::cout << "read json failed" << std::endl;
	// 	return ret;
	// }
	// ret = checkJsonFormat(doc);
	// if (ret != 0) {
	// 	std::cout << "checkConfigJsonFormat failed" << std::endl;
	// 	return ret;
	// }
	//读取配置文件中的属性
	
	//设置调试等级
	//控制log文件的大小
	Logger* log = Logger::getInstance();
	std::string str = "std::cout << checkConfigJsonFormat failed << std::endl;";
	std::thread t(fun,log,std::ref(str));
	t.join();

	return 0;
}

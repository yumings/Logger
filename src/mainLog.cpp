#include <Logger.h>
#include <thread>
#include <string>


//
void fun(Logger* log,int a)
{
    for (int i = 0; i < 1000; ++i)
    {
        std::string info = "thread:" + std::to_string(a);
        log->TraceInfo(info);
    }
}


int main(int argc,char* argv[])
{
	Logger* log = Logger::getInstance();


	log->TraceInfo("image has changed");	
	log->TraceInfo("image has changed",LogLevel::Log_Warning);	
    //∂‡œﬂ≥Ãlog

    std::thread thread1(fun,log,10);
    std::thread thread2(fun,log,20);

    thread1.join();
    thread2.join();
    
	return 0;
}

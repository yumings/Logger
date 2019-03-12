# logger
c++ windows/linux logger 

linux下编译的库
动态库的编译
开启CMakeLists.txt中的动态编译选项
#mkdir build & cd build & cmake ../
#make -j6
可以得到编译的动态库liblog.so
开启CMakeLists.txt中的静态编译选项
可以得到编译的静态库liblog.a

动态库和静态库的调用

#g++ mainLog.cpp -Lbuild -llog -lpthread -Iinclude
#export LD_LIBRARY_PATH=./build



【功能简介】
本工程使用OnVif协议操作IPC，主要有如下功能：
（1）能否主动发现局域网内连接的IPC设备并获取该设备的基础信息；
（2）获取IPC设备的RTSP流地址；
（3）控制PTZ云台，如左右、变倍聚焦等；

【使用方法】
参照example/main.cpp

【编译方法】
默认：该工程默认已经集成到了如下仓库
ssh://git@192.168.1.31:10318/syvb/rockchip/rv1126.git
分支：videobox_tp
在rv1126的源码根目录中运行如下命令：
make IPCOnvif-dirclean
make IPCOnvif-rebuild

backup目录中有如下三个文件，根据你的编译环境选择
（1）RV1126的buildroot环境中编译，请使用CMakeLists_buildroot_compile.txt
（2）RV1126的交叉编译环境，请使用CMakeLists_cross_compile.txt
如将该工程预置到rv1126源码根目录的app目录中，进入该工程的build目录，运行cmake ..和make即可编译
（3）ubuntu的环境编译，请使用CMakeLists_ubuntu_compile.txt
需要安装ffmpeg
进入该工程的build目录，运行cmake ..和make即可编译

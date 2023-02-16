# CMakeList.txt: ffmpeg 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
set(INC "/usr/include/x86_64-linux-gnu/" "/usr/include/mysql-cppconn-8"  "/usr/include" "/home/xingxing/libvlcpp/libvlcpp-master/libvlcpp-master")
set(LIBS "/usr/lib/x86_64-linux-gnu")
include_directories(${INC})
link_directories(${LIBS})
include_directories("/home/xingxing/libevent/build/include")
link_directories("/home/xingxing/libevent/build/lib")
link_libraries("event")
 #event_core event_openssl event_pthreads event_extra
#include_directories("/home/xingxing/SDL/SDLLIBS/include")
#link_directories("/home/xingxing/SDL/SDLLIBS/lib")
#libmysqlclient
#include_directories("/usr/include")
#link_directories("/usr/lib/x86_64-linux-gnu")
#link_libraries("mysqlclient")

# 将源代码添加到此项目的可执行文件。
add_executable (ffmpeg "ffmpeg.cpp" "ffmpeg.h" "JT1078Header.h" "H264/h264decoder.h" "Deprotocal/Deprotocal.h" 
                         "Deprotocal/Deprotocal.cpp" "CCodec/type.h" "CCodec/CCodec.h" "CCodec/CCodec.cpp" "CCodec/codec/g726.h" "CCodec/codec/g726.cpp" 
                         "CCodec/codec/Adpcm.h" "CCodec/codec/Adpcm.cpp" "CCodec/codec/G711.h" "CCodec/codec/G711.cpp" 
                         "Buffer/Buffer.h" "Buffer/Buffer.cpp")
target_link_libraries(ffmpeg  "avcodec" "avformat" "avutil" "avdevice" "avfilter" "postproc" "swresample" "swscale")
target_link_libraries(ffmpeg "mysqlcppconn8")
target_link_libraries(ffmpeg "vlc")
#target_link_libraries(ffmpeg "SDL2" "SDL2test")
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ffmpeg PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

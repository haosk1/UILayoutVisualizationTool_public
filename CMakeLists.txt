#指定cmake版本 
cmake_minimum_required(VERSION 3.2)

#指定C++版本和编译选项
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")#支持C++11
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g -ggdb")#debug版本 -O0：无优化 -g:生成并在二进制文件中嵌入调试符号 -ggdb:允许gdb调试
set(CMAKE_CXX_FLAGS_RELEASE "-O3")#release版本 -O3：最大程度优化。
# set(CMAKE_CXX_FLAGS  "-fsanitize=address")#sanitizer 性能减半，并且不会提示warning了
# set(CMAKE_CXX_FLAGS  "-fno-omit-frame-pointer")#sanitizer 为了获得更友好的栈追踪信息

#project name，指定项目的名称，一般和项目的文件夹名称对应
#会定义两个变量：PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR。
# ${PROJECT_SOURCE_DIR}：本CMakeLists.txt所在的文件夹路径
# ${PROJECT_NAME}：本CMakeLists.txt的project名称
project(haosk_project)

# #添加包含的子cmakelist所在目录
# add_subdirectory(${PROJECT_SOURCE_DIR}/frame frame)

#源文件头文件包含的文件目录
include_directories(${PROJECT_SOURCE_DIR}/library/baselib/Include)
include_directories(${PROJECT_SOURCE_DIR}/library/frame/Include)
include_directories(${PROJECT_SOURCE_DIR}/UIWork)

#查找目录下的所有源文件，并将名称保存到 DIR_SRCS 变量
aux_source_directory(${PROJECT_SOURCE_DIR}/main DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/UIWork DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/UIWork/Form DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/UIWork/Stage DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/UIWork/Actor DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/UIWork/AttributePlug DIR_SRCS)

#可以提前link
# link_libraries(${PROJECT_SOURCE_DIR}/baselib/libbaselib.so)
# link_libraries(${PROJECT_SOURCE_DIR}/frame/libframe.so)

#将所有源文件编译成可执行程序，var1即程序名
add_executable(${PROJECT_NAME} ${DIR_SRCS})

#添加可执行文件所需要的库
target_link_libraries(${PROJECT_NAME} PRIVATE -lfreetype)
target_link_libraries(${PROJECT_NAME} PRIVATE -lpthread)
target_link_libraries(${PROJECT_NAME} PRIVATE -lstdc++fs)
# target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/library/baselib/libbaselib.so)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/library/frame/libframe.so)
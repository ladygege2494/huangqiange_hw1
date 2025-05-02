# 设置最低 CMake 版本
cmake_minimum_required(VERSION 4.0)

# 项目名称
project(main)

# 设置可执行文件的输出目录
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加包含目录
include_directories (inc)

# 查找源文件
aux_source_directory (src SRC_LIST)

# 添加可执行文件
add_executable (main ${SRC_LIST})
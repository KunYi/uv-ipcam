project(IPCOnvif)
cmake_minimum_required(VERSION 3.8)

set(C_COMPILER "${RV1126_OUTPUT_DIR}/host/bin/arm-linux-gnueabihf-gcc")
set(CXX_COMPILER "${RV1126_OUTPUT_DIR}/host/bin/arm-linux-gnueabihf-g++")
set(SYS_ROOT "${RV1126_OUTPUT_DIR}/host/arm-buildroot-linux-gnueabihf/sysroot")

SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER ${C_COMPILER})
set(CMAKE_CXX_COMPILER ${CXX_COMPILER})
SET(CMAKE_FIND_ROOT_PATH ${SYS_ROOT})

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-fPIC)
add_compile_options(-std=c++11 -Wno-psabi -Wall -Wextra -Wpedantic -Wno-unused-parameter)

############################################

aux_source_directory(src DIR_SRCS)
aux_source_directory(example DIR_SRCS)
aux_source_directory(include DIR_SRCS)
add_library(${PROJECT_NAME} SHARED
    ${DIR_SRCS}
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/
)

add_definitions(-DWITH_NONAMESPACES -DWITH_DOM -DWITH_OPENSSL)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
#install(FILES ${HEADER_SRCS} DESTINATION include/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.*")


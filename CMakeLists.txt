cmake_minimum_required(VERSION 3.15)
PROJECT(IGL)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -O3")
include_directories(./src)
include_directories(./run)

aux_source_directory(./src INCLUDES)
#set(RUNS ./run/exp.cpp)


add_library(INCLUDES_LIB ${INCLUDES})
#add_library(RUNS_LIB ${RUNS})

add_executable(para ./run/para.cpp)
target_link_libraries(para INCLUDES_LIB)

add_executable(IG ./run/runIG.cpp)
target_link_libraries(IG INCLUDES_LIB)

add_executable(IGLPlus ./run/runIGLPlus.cpp)
target_link_libraries(IGLPlus INCLUDES_LIB)

add_executable(IGL_PRPlus ./run/runIGL_PRPlus.cpp)
target_link_libraries(IGL_PRPlus INCLUDES_LIB)

add_executable(IGHeap ./run/runIGHeap.cpp)
target_link_libraries(IGHeap INCLUDES_LIB)

add_executable(MaxD ./run/runMaxD.cpp)
target_link_libraries(MaxD INCLUDES_LIB)

add_executable(IGLPlusT ./run/runIGLPlus_withTree.cpp)
target_link_libraries(IGLPlusT INCLUDES_LIB)

add_executable(IGLPlusIns ./run/runIGLPlus_insert.cpp)
target_link_libraries(IGLPlusIns INCLUDES_LIB)

add_executable(IGLPlusRem ./run/runIGLPlus_remove.cpp)
target_link_libraries(IGLPlusRem INCLUDES_LIB)
#Copyright 2019 Husky Data Lab, CUHK
#Authors: Hongzhi Chen

cmake_minimum_required(VERSION 3.3.0)
project(GRASPER)

set(CMAKE_EXPORT_COMPILE_COMMANDS  1) 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release/)

#CONFIGURE ME!!!
#Please manually SET the paths for MPI C/CXX compiler.
set(CMAKE_C_COMPILER   "/home/s164864/home/mpich/bin/mpicc")
set(CMAKE_CXX_COMPILER "/home/s164864/home/mpich/bin/mpic++")

set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
# close the debug mode
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(COMMON_LINK_LIBS "-lrt")


#CONFIGURE ME!!!
#Please manually SET the dependency paths for the following libraries.
set(ZMQ_ROOT "/home/s164864/home/zmq")
set(GLOG_ROOT "/home/s164864/home/glog/")
set(HDFS2_ROOT "/home/s164864/home/hadoop-2.6.0")
set(IBVERB_ROOT "/home/s164864/home/lib/libibverbs")
set(TBB_ROOT "/home/s164864/intel/oneapi/tbb/latest")
set(MKL_ROOT "/home/s164864/intel/oneapi/mkl/latest")

set(ZMQ_INCLUDE_DIR "/home/s164864/home/lib/libzmq/include")
set(ZMQ_LIBRARIES "/home/s164864/home/lib/libzmq/lib64/libzmq.so")
set(GLOG_INCLUDE_DIR "/home/s164864/home/glog/include")
set(GLOG_LIBRARIES "/home/s164864/home/glog/lib64/libglog.so")
set(TBB_INCLUDE_DIR "/home/s164864/intel/oneapi/tbb/latest/include")
set(TBB_LIBRARIES "/home/s164864/intel/oneapi/tbb/latest/lib/intel64/gcc4.8/libtbb.so")
set(MKL_INCLUDE_DIR "/home/s164864/intel/oneapi/mkl/latest/include")
add_definitions(-DUSE_HDFS2_)  #use hdfs v2.x.x by defalut
add_definitions(-DHAS_RDMA)  #use RDMA

find_package(Threads)

# External Dependencies
include(cmake/modules/glog.cmake)
include(cmake/modules/hdfs.cmake)
include(cmake/modules/ibverbs.cmake)
include(cmake/modules/zmq.cmake)
include(cmake/modules/tbb.cmake)
# MKL
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MKL_ROOT}/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${MKL_ROOT}/lib/intel64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lmkl_blas95_lp64 -lmkl_lapack95_lp64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core ")

set(DEPS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(GRASPER_EXTERNAL_INCLUDES  ${GLOG_INCLUDE_DIR} ${HDFS2_INCLUDE_DIR} ${IBVERB_INCLUDE_DIR}  ${ZMQ_INCLUDE_DIR} ${TBB_INCLUDE_DIR} ${DEPS_INCLUDE_DIR})
set(GRASPER_EXTERNAL_LIBRARIES ${GLOG_LIBRARIES} ${HDFS2_LIBRARIES} ${IBVERB_LIBRARIES} ${ZMQ_LIBRARIES} ${TBB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${COMMON_LINK_LIBS})
set(PUT_EXTERNAL_LIBRARIES ${HDFS2_LIBRARIES} ${COMMON_LINK_LIBS})

INCLUDE_DIRECTORIES(expert)
add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(storage)
add_subdirectory(utils)
add_subdirectory(third_party)
add_subdirectory(develop)
add_subdirectory(driver)
add_subdirectory(put)

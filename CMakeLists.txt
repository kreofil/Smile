# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(smile LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS OFF)

#find_package(Boost 1.72 REQUIRED program_options)
#include_directories(${Boost_INCLUDE_DIRS})
#add_definitions(-DBOOST_ALL_NO_LIB)

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}/../lib")

message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
  message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
  message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

#set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type is Release" FORCE)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

#add_executable(smile main.cpp)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(BEFORE
    src/smodel
)

#target_link_libraries(smile ${Boost_LIBRARIES})
#target_compile_features(smile PRIVATE cxx_auto_type cxx_range_for)
add_subdirectory(src/compiler)
add_subdirectory(src/expcompiler)
add_subdirectory(src/launcher)
add_subdirectory(src/smodel)

#target_link_libraries(smile "src/compiler")     # Линковка программы с библиотекой

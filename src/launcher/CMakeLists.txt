# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
#project(smile LANGUAGES CXX)

find_package(Boost 1.72 REQUIRED program_options)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_ALL_NO_LIB)

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

add_executable(smile main.cpp)

target_link_libraries(smile ${Boost_LIBRARIES})
target_compile_features(smile PRIVATE cxx_auto_type cxx_range_for)

target_link_libraries(smile util)         # Линковка программы с библиотекой утилит 
target_link_libraries(smile compiler)     # Линковка программы с библиотекой компилятора артефактов
target_link_libraries(smile expcompiler)  # Линковка программы с библиотекой компилятора экспорта
target_link_libraries(smile smodel)       # Линковка программы с библиотекой семантической модели

# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#set(BUILD_DIR "${CMAKE_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

# Установка переменной со списком исходников
set(SOURCE_LIB 
    # headers
    #expcomp.h
    parser.h
    
    #sources
    #expcomp.cpp
    util.cpp
    tlt.cpp
    scanner.cpp
    parser.cpp
)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

add_library(compiler STATIC ${SOURCE_LIB})   # Создание статической библиотеки

target_link_libraries(compiler smodel)       # Линковка библиотеки с библиотекой семантической модели

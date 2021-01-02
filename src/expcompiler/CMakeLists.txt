# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

#set(BUILD_DIR "${CMAKE_BINARY_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

# Установка переменной со списком исходников
set(SOURCE_LIB 
    # headers
    parser.h
    
    #sources
    util.cpp
    tlt.cpp
    scanner.cpp
    parser.cpp
)

add_library(expcompiler STATIC ${SOURCE_LIB})   # Создание статической библиотеки

target_link_libraries(expcompiler smodel)       # Линковка библиотеки с библиотекой семантической модели

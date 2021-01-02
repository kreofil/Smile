# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Установка переменной со списком исходников
set(SOURCE_LIB 
    # headers
    const.h
    type.h
    func.h
    declaration.h
    nametable.h
    funcdeclaration.h
    tuple.h
    actor.h
    smodel.h
    
    #sources
    const.cpp
    type.cpp
    func.cpp
    declaration.cpp
    nametable.cpp
    funcdeclaration.cpp
    tuple.cpp
    actor.cpp
    smodel.cpp
)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

add_library(smodel STATIC ${SOURCE_LIB})   # Создание статической библиотеки


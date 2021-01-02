#include <iostream>

#include "declaration.h"

Declaration::Declaration()
{}

Declaration::Declaration(std::string n): name(n)
{}

// Тестовый вывод экспортируемого объявления в консоль
void Declaration::DebugOut() {
    std::cout << "Declaration: " << name << " ---> ";
}

DeclarationConst::DeclarationConst(Const* cv): constValue{cv}
{}

DeclarationConst::DeclarationConst(std::string n, Const* cv): 
    Declaration{n}, constValue{cv}
{}

// Формирование объявления как текстовой строки
std::string DeclarationConst::toString() {
    return constValue->toString();
}

// Тестовый вывод экспортируемого объявления в консоль
void DeclarationConst::DebugOut() {
    Declaration::DebugOut();
    constValue->DebugOut();
//     std::cout << "\n";
}

DeclarationType::DeclarationType(Type* tv): typeValue{tv}
{}

DeclarationType::DeclarationType(std::string n, Type* tv): 
    Declaration{n}, typeValue{tv}
{}

// Формирование объявления как текстовой строки
std::string DeclarationType::toString() {
    std::string s{"type "};
    s += typeValue->toString();
    return s;
}

// Тестовый вывод экспортируемого объявления в консоль
void DeclarationType::DebugOut() {
    Declaration::DebugOut();
    std::cout << "type ";
    typeValue->DebugOut();
//     std::cout << "\n";
}

// Сравнение типов на структурную эквивалентность
bool DeclarationType::EqStruct(DeclarationType *pt) {
    return typeValue->EqStruct(pt->GetType());
}

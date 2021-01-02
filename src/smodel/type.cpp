#include <iostream>
#include <sstream>

#include "type.h"

Type::Type() {}

// Сравнение типов на структурную эквивалентность
bool Type::EqStruct(Type *pt) {
    return this == pt;
}

// Общий вывод для всех типов
//std::string Type::toString() {
//    return "type ";
//}

// Тестовый вывод общего описания всех констант
//void Type::DebugOut() {
//    std::cout << "type ";
//}

TypeSignal::TypeSignal() {}

// Вывод строки для сигнального типа
std::string TypeSignal::toString() {
    // std::string s = Type::toString();
    std::string s{"signal"};
    return s;
}

// Тестовый вывод сигнального типа
void TypeSignal::DebugOut() {
    // Type::DebugOut();
    std::cout << "signal";
}

TypeInt::TypeInt() {}

// Вывод строки для целочисленного типа
std::string TypeInt::toString() {
    // std::string s = Type::toString();
    std::string s{"int"};
    return s;
}

// Тестовый вывод целочисленного типа
void TypeInt::DebugOut() {
    // Type::DebugOut();
    std::cout << "int";
}

TypeBool::TypeBool() {}

// Вывод строки для булевского типа
std::string TypeBool::toString() {
    // std::string s = Type::toString();
    std::string s{"bool"};
    return s;
}

// Тестовый вывод булевского типа
void TypeBool::DebugOut() {
    // Type::DebugOut();
    std::cout << "bool";
}

TypeFloat::TypeFloat() {}

// Вывод строки для действительного типа
std::string TypeFloat::toString() {
    // std::string s = Type::toString();
    std::string s{"float"};
    return s;
}

// Тестовый вывод действительного типа
void TypeFloat::DebugOut() {
    // Type::DebugOut();
    std::cout << "float";
}

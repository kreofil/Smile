#include <iostream>
#include <sstream>

#include "const.h"

Const::Const() {}

// Общий вывод для всех констант
std::string Const::toString() {
    return "const ";
}

// Тестовый вывод общего описания всех констант
void Const::DebugOut() {
    std::cout << "const ";
}

// Вывод строки для сигнальной константы
std::string ConstSignal::toString() {
    std::string s = Const::toString();
    std::ostringstream os;
    os <<  "signal";
    s += os.str();
    return s;
}

// Тестовый вывод сигнальной константы
void ConstSignal::DebugOut() {
    Const::DebugOut();
    std::cout << "signal = !";
}


ConstInt::ConstInt(int v): value{v}
{}

// Вывод строки для целочисленных констант
std::string ConstInt::toString() {
    std::string s = Const::toString();
    std::ostringstream os;
    os <<  "int " << value;
    s += os.str();
    return s;
}

// Тестовый вывод целочисленных констант
void ConstInt::DebugOut() {
    Const::DebugOut();
    std::cout << "int = " << value;
}

ConstBool::ConstBool(bool v): value{v}
{}

// Вывод строки для булевских констант
std::string ConstBool::toString() {
    std::string s = Const::toString();
    s += std::string("bool ") + std::string(value? "true": "false");
    return s;
}

// Тестовый вывод для булевских констант
void ConstBool::DebugOut() {
    Const::DebugOut();
    std::cout << "bool = " << (value? "true": "false");
}

ConstFloat::ConstFloat(double v): value{v}
{}

// Вывод строки для действительных констант
std::string ConstFloat::toString() {
    std::string s = Const::toString();
    std::ostringstream os;
    os <<  "float " << value;
    s += os.str();
    return s;
}

// Тестовый вывод для действительных констант
void ConstFloat::DebugOut() {
    Const::DebugOut();
    std::cout << "float = " << value;
}


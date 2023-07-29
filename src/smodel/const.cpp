#include <iostream>
#include <sstream>

#include "const.h"

//-----------------------------------------------------------------------------
// Вспомогательная функция, формирующая вывод символа при наличии слэша
void slashOut(char ch, std::string &str) {
    // Символы, недопустимые без слэша
    if( ch == '\\') { // обратная косая (\)
        str = "\\\\";
    } else if(ch == '\'') { // апостроф (')
        str = "\\\'";
    } else if(ch == '"') { // кавычка (")
        str = "\\\"";
    } else if(ch == '\t') { // табуляция (\t)
        str = "\\t";
    } else if(ch == '\n') { // перевод строки (\n)
        str = "\\n";
    } else {
        str = ch;
    }
}

//-----------------------------------------------------------------------------
Const::Const() {}

// Общий вывод для всех констант
std::string Const::toString() {
    return "const ";
}

// Тестовый вывод общего описания всех констант
void Const::DebugOut() {
    std::cout << "const ";
}

//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
ConstChar::ConstChar(char v): value{v}
{}

// Вывод строки для символьных констант
std::string ConstChar::toString() {
    std::string s = Const::toString();
    std::ostringstream os;
    std::string out_value{""};
    // Символы, недопустимые без слэша
    slashOut(value, out_value);
    os <<  "char '" << out_value << "'";
    s += os.str();
    return s;
}

// Тестовый вывод символьных констант
void ConstChar::DebugOut() {
    Const::DebugOut();
    std::string out_value{""};
    // Символы, недопустимые без слэша
     slashOut(value, out_value);
    std::cout << "char = '" << out_value << "'";
}

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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


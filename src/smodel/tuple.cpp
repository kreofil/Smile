#include <iostream>
#include <sstream>

#include "tuple.h"

TypeTuple::TypeTuple() {}

// Вывод строки для кортежа
std::string TypeTuple::toString() {
    // std::string s = Type::toString();
    std::string s{"tuple "};
    // Формируется число, определяющее длину кортежа
    auto len = elem.size();
    // и добавляется к строке вывода
    std::ostringstream os;
    os << len;
    s += os.str();
    // И осуществляется вывод типов элементов кортежа
    for(auto it: elem) {
        s += " " + it->GetType()->toString();
    }
    return s;
}

// Тестовый вывод кортежа
void TypeTuple::DebugOut() {
    // Type::DebugOut();
    std::cout << "tuple (";
    // Осуществляется вывод типов элементов кортежа
    auto flag = false;
    for(auto it: elem) {
        if(flag) {
            std::cout << ",";
        }
        flag = true;
        it->GetType()->DebugOut();
    }
    std::cout << ")";
}



// Определение объема памяти для кортежа (по объему данных)
int TypeTuple::TypeSize() {
    auto memSize = 0;
    for(auto it: elem) {
        memSize += it->GetType()->TypeSize();
    }
    return memSize;
}

// Сравнение типов на структурную эквивалентность
bool TypeTuple::EqStruct(Type *pt) {
    if(TypeTuple* ptt = dynamic_cast<TypeTuple*>(pt); ptt != nullptr) {
        if(Size() != ptt->Size()) {
            return false;
        }
        for(auto i = 0; i < Size(); i++) {
            if(!Element(i)->EqStruct(ptt->Element(i))) {
                return false;
            }
        }
        return true;
    }
    return false;
}


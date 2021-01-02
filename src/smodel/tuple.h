#ifndef __tuple__
#define __tuple__
// Описание кортежа

#include "declaration.h"

// Определение кортежа
class TypeTuple: public Type {
    // Типы отдельных элементов кортежа
    std::vector<DeclarationType*> elem;
public:
    explicit TypeTuple();
    virtual std::string toString();
    virtual void DebugOut();
    virtual int TypeSize();
    // Сравнение типов на структурную эквивалентность
    virtual bool EqStruct(Type *pt);

    // Добавление очередного типа к кортежу
    void AddType(DeclarationType* dt) {elem.push_back(dt);}
    // Получение i-го элемента кортежа
    DeclarationType* Element(int i) {return elem[i];}
    // Получение длины кортежа
    int Size() {return elem.size();}
};

#endif


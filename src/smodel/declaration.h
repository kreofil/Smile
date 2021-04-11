#ifndef __declaration__
#define __declaration__

#include <string>

#include "const.h"
#include "type.h"

// Класс, определяющий объявление формируемых артефактов (абстрактный)
class Declaration {
    // Указатель на свое собственное имя (для более быстрого доступа при необходимости)
    std::string name;
public:
    explicit Declaration();
    explicit Declaration(std::string n);

    virtual std::string toString() = 0;
    virtual void DebugOut();

    std::string& GetName() {return name;}
    void SetName(std::string n) {name = n;}
};

// Класс, определяющий объявление одной из констант
class DeclarationConst: public Declaration {
    Const* constValue;
public:
    explicit DeclarationConst(Const* cv);
    explicit DeclarationConst(std::string n, Const* cv);

    virtual std::string toString();
    virtual void DebugOut();
    
    Const* GetConst() {return constValue;}
};

// Класс, определяющий объявление одного из типов
class DeclarationType: public Declaration {
    Type* typeValue;
public:
    explicit DeclarationType(Type* tv);
    explicit DeclarationType(std::string n, Type* tv);

    virtual std::string toString();
    virtual void DebugOut();
    
    Type* GetType() {return typeValue;}
    
    // Сравнение типов на структурную эквивалентность
    bool EqStruct(DeclarationType *pt);
};

#endif

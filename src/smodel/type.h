#ifndef __type__
#define __type__
// Описание типов семантической модели

#include <vector>

// Класс, задающий обобщенный тип
class Type {
public:
    explicit Type();
    virtual std::string toString() = 0;
    virtual void DebugOut() = 0;
    virtual int TypeSize() = 0;

    // Сравнение типов на структурную эквивалентность
    virtual bool EqStruct(Type *pt);
};

// Определение сигнального типа
class TypeSignal: public Type {
public:
    explicit TypeSignal();
    virtual std::string toString();
    virtual void DebugOut();
    virtual int TypeSize() {return sizeof(bool);};
};

// Определение целочисленного типа
class TypeInt: public Type {
public:
    explicit TypeInt();
    virtual std::string toString();
    virtual void DebugOut();
    virtual int TypeSize() {return sizeof(int);};
};

// Определение булевского типа
class TypeBool: public Type {
public:
    explicit TypeBool();
    virtual std::string toString();
    virtual void DebugOut();
    virtual int TypeSize() {return sizeof(bool);}
};

// Определение действительного типа
class TypeFloat: public Type {
public:
    explicit TypeFloat();
    virtual std::string toString();
    virtual void DebugOut();
    virtual int TypeSize() {return sizeof(double);}
};

#endif


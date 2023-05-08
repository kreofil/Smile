#ifndef __const__
#define __const__

// Представление в семантической модели констант

#include <string>

#include "utf8.h"

// Класс, определяющий обобщенную константу
class Const {
public:
    explicit Const();
    virtual std::string toString();
    virtual void DebugOut();
};

// Представление сигнальной константы
class ConstSignal: public Const {
public:
    explicit ConstSignal() {}
    virtual std::string toString();
    virtual void DebugOut();
};

// Представление целочисленной константы
class ConstInt: public Const {
    int value;
public:
    explicit ConstInt(int v);
    virtual std::string toString();
    virtual void DebugOut();
};

// Представление булевской константы
class ConstBool: public Const {
    bool value;
public:
    explicit ConstBool(bool v);
    virtual std::string toString();
    virtual void DebugOut();
};

// Представление действительной константы
class ConstFloat: public Const {
    double value;
public:
    explicit ConstFloat(double v);
    virtual std::string toString();
    virtual void DebugOut();
};

// Представление константы-руны
class ConstRune : public Const {
    rune value;
public:
    explicit ConstRune(rune r);
    std::string toString() override;
    void DebugOut() override;
};

#endif

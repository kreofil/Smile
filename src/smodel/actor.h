#ifndef __actor__
#define __actor__

#include <fstream>
#include <string>
//#include <map>
#include <vector>

#include "declaration.h"
//#include "nametable.h"
//#include "tuple.h"

//==============================================================================
// Базовый класс, определяющий общие поля и методы для любой вершины
// реверсивного информационного графа (РИГ)
class Actor {
    // Номер вершины, формируемый во время компиляции
    int number;
    // Указатель на объявление обозначения, являющегося выходом актора
    Declaration* outDeclaration;
    // Указатель на следующий актор по отношению расположения
    // (Может не понадобитья, если не фиксировать явно порядок следования.)
    Actor* next;
public:
    explicit Actor(int n = -1): number{n} {}
    explicit Actor(Declaration* pd, int n = -1): number{n}, outDeclaration{pd}
    {}
    
    virtual void DebugOut();
    
    // Получение номера вершины
    int GetNumber() {return number;}
    // Установка номера вершины
    void SetNumber(int n) {number = n;}
    // Получение обозначения на выходе актора
    Declaration* GetOutDeclaration() {return outDeclaration;}
    // Установка обозначения на выходе актора
    void SetOutDeclaration(Declaration* pd) {outDeclaration = pd;}
    // Получение указателя на следующий актор
    Actor* GetNext() {return next;}
    // Установка указателя на следующий актор
    void SetNext(Actor* pa) {next = pa;}
};

//==============================================================================
// Начальная вершина РИГ, содержащая ссылку на объявление аргумента
class ActorArg: public Actor {
public:
    explicit ActorArg(int n = -1): Actor{n} {}
    explicit ActorArg(Declaration* pd, int n): Actor{pd, n} {}

    virtual void DebugOut();
};

//==============================================================================
// Конечная вершина РИГ, содержащая ссылку на объявление результата
class ActorRet: public Actor {
    // Указатель на актор, выступающий в роли источника данных
    Actor* in;
public:
    explicit ActorRet(int n = -1): Actor{n} {}
    explicit ActorRet(Declaration* pd, int n = -1): Actor{pd, n} {}

    virtual void DebugOut();
};

//==============================================================================
// Вершина, задающая одноаргументный оператор интерпретации
class ActorInterpret: public Actor {
    // Указатель на актор, выступающий в роли аргумента
    Actor* arg;
    // Указатель на актор, выступающий в роли функции
    Actor* func;
public:
    explicit ActorInterpret(int n = -1): Actor{n} {}
    explicit ActorInterpret(Declaration* pd, int n = -1): Actor{pd, n} {}

    virtual void DebugOut();
    
    // Установка указателя на актор - аргумент
    void SetArg(Actor* pa) {arg = pa;}
    // Установка указателя на актор - функцию
    void SetFunc(Actor* pf) {func = pf;}
};

//==============================================================================
// Вершина, задающая массовый оператор интерпретации
class ActorInterpretMass: public Actor {
    // Указатель на актор, выступающий в роли аргумента
    Actor* arg;
    // Указатель на актор, выступающий в роли функции
    Actor* func;
public:
    explicit ActorInterpretMass(int n = -1): Actor{n} {}
    explicit ActorInterpretMass(Declaration* pd, int n = -1): Actor{pd, n} {}

    virtual void DebugOut();

    // Установка указателя на актор - аргумент
    void SetArg(Actor* pa) {arg = pa;}
    // Установка указателя на актор - функцию
    void SetFunc(Actor* pf) {func = pf;}
};

//==============================================================================
// Вершина, определяющая константное значение любого типа
// Содержит уже прикрепленную константу, 
// доступную через объявление константы другим акторам
class ActorConst: public Actor {
public:
    explicit ActorConst(int n = -1): Actor{n} {}
    explicit ActorConst(Declaration* pd, int n): Actor{pd, n} {}

    virtual void DebugOut();
};

//==============================================================================
// Вершина, определяющая кортеж
class ActorTuple: public Actor {
    std::vector<Actor*> elements;
public:
    explicit ActorTuple(int n = -1): Actor(n) {}
    explicit ActorTuple(Declaration *pd, int n): Actor(pd, n) {}

    void DebugOut() override;

    void AddElement(Actor *pa);
    size_t GetElementCount() const;
    Actor* GetElement(size_t i) const;
};

//==============================================================================
class ActorSwarm: public Actor {
};

//==============================================================================
class ActorStream: public Actor {
};

//==============================================================================
class ActorFunc: public Actor {
public:
    explicit ActorFunc(int n = -1): Actor{n} {}
    explicit ActorFunc(Declaration* pd, int n): Actor{pd, n} {}

    virtual void DebugOut();
};

//==============================================================================
// Класс объявляющий имя, связанное с актором
class DeclarationActor: public Declaration {
    Actor* actorValue;
public:
    explicit DeclarationActor(Actor* av);
    explicit DeclarationActor(std::string n, Actor* av);

    virtual std::string toString();
    virtual void DebugOut();
    
    Actor* GetActor() {return actorValue;}
    
};
#endif

#ifndef __func__
#define __func__

#include <vector>
//#include <map>

#include "nametable.h"
#include "actor.h"

/*
 * Классы объявления и описания функции раздельны. Первый описывает прототип,
 * который возвращается из файла экспорта и формируется при компиляции
 * функции перед вставкой в файл экспорта.
 * Класс описания формируется в том случае, когда функция имеет тело. 
 * То есть, только тогда, когда она не позиционируется в качестве прототипа.
*/

//==============================================================================
// Сигнатуры функций, выделенные в отдельный класс.
// Включает тип аргумента и результата
class Signature {
    DeclarationType* argDeclarationType;  // Объявление типа аргумента
    DeclarationType* resDeclarationType;  // Объявление типа результата
public:
    explicit Signature(DeclarationType* adt, DeclarationType* rdt);
    void DebugOut();
    std::string toString();
    
    DeclarationType* GetArgDeclarationType() {return argDeclarationType;}
    DeclarationType* GetResDeclarationType() {return resDeclarationType;}
    
    int Cmp(Signature& s);
};


//==============================================================================
// Класс, задающий объявление функции
class FuncProto {
    // Список сигнатурных типов (для перегруженной функции)
    std::vector<Signature> sigList;
public:
    explicit FuncProto();
    
    std::string toString();
    void DebugOut();
    
    // Получение ссылки на список типов аргументов
    std::vector<Signature>& GetSignatures() {return sigList;}
    // Добавление очередного типа аргумента в список сигнатурных типов
    void AddSig(Signature s) {sigList.push_back(s);}
};

//==============================================================================
// Класс, задающий описание функции
class Func {
    // Имя типа аргумента (если оно присутствует)
    std::string argTypeName;
    // Таблица локальных имен функции
    //std::vector<Declaration*> localNameTable;
    NameTable localNameTable;
    // Таблица имен, импортируемых из таблицы экспорта
    std::vector<Declaration*> importNameTable;
    // Типы аргумента и результата
    Type* argType;  // стилизован для структурной эквивалентности
    Type* resType;
    // Список вершин РИГ, сформированных при компиляции тела функции
    std::vector<Actor*> body;
public:
    explicit Func();
    
    std::string toDclString();
    //std::string toBodyString();
    void DebugOut();
    
    //------------------------------------------------------------------
    // Добавление в таблицу локальныхимен функции очередного объявления
    void AddToLocalNameTable(Declaration* pd);
    // Добавление в таблицу импортируемых имен
    void AddToImportNameTable(Declaration* pd);
    
    // Поиск объявления актора в локальной таблице имен
    DeclarationActor* FindLocalDeclaration(std::string& n);

    // Получение имени аргумента функции
    std::string& GetArgTypeName() {return argTypeName;}
    // Установка имени аргумента функции
    void SetArgTypeName(std::string n) {argTypeName = n;}
    // Установка типа аргумента
    void SetArgType(Type* t) {argType = t;}
    // Получение типа аргумента
    Type* GetArgType() {return argType;}
    // Установка типа результата
    void SetResType(Type* t) {resType = t;}
    // Получение типа результата
    Type* GetResType() {return resType;}
    
    //------------------------------------------------------------------
    // Добавление вершины в РИГ
    void AddActor(Actor* pa) {body.push_back(pa);}
    // Количество вершин в РИГ
    int ActorNumber() {return std::size(body);}
    // Возврат указателя на актор по индексу
    Actor* GetActor(int index) {return body[index];}
};

#endif

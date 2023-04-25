#ifndef __smodel__
#define __smodel__

#include <fstream>
#include <string>
//#include <map>
#include <vector>

//#include "declaration.h"
#include "funcdeclaration.h"
#include "tuple.h"
#include "struct.h"
#include "nametable.h"

// Семантическая модель языка программирования Smile
class SemanticModel {
    // Имя компилируемого артефакта
    std::string artefactName;
    // Таблица имен для предопределенного импорта,
    // существующего внутри компилятора
    NameTable internalNameTable;
    // Таблица имен для экспортируемых артефактов
    NameTable exportNameTable;
    // Указатель на компилируемую функцию.
    // Устанавливается, когда осуществляется ее компиляция
    Func* compiledFunction;
    
public:
    explicit SemanticModel();
    
    // Тестовый вывод содержимого семантической модели в консоль
    void DebugOut();
    
    //------------------------------------------------------------------
    // Установка имени компилируемого артефакта
    void SetArtefactName(const std::string& an) {artefactName = an;}
    // Получение имени компилируемого артефакта
    std::string& GetArtefactName() {return artefactName;}
    
    //------------------------------------------------------------------
    // Формирование указателя на сигнальную константу
    Const* NewSignalValue();
    // Создание целочисленной константы
    Const* NewIntValue(int iv);
    // Создание булевской константы
    Const* NewBoolValue(bool bv);
    // Создание действительной константы
    Const* NewFloatValue(double fv);
    
    //------------------------------------------------------------------
    // Формирование объявление полученной обобщенной константы
    Declaration* newDeclarationConst(Const* c);
    // Формирование объявления целочисленной константы
    Declaration* newDeclarationConstInt(int v);
    // Формирование объявления булевской константы
    Declaration* newDeclarationConstBool(bool v);
    // Формирование объявления действительной константы
    Declaration* newDeclarationConstFloat(double v);

    //------------------------------------------------------------------
    // Получение указателя на сигнальный тип
    Type* GetTypeSignal();
    // Получение указателя на целочисленный тип
    Type* GetTypeInt();
    // Получение указателя на булевский тип
    Type* GetTypeBool();
    // Получение указателя на действительный тип
    Type* GetTypeFloat();

    //------------------------------------------------------------------
    // Формирование объявления для сформированного типа
    DeclarationType* newDeclarationType(Type* t);
    
    //------------------------------------------------------------------
    // Создание прототипа функции
    FuncProto* NewFuncProto();
    // Создание функции
    Func* NewFunc();
    // Создание кортежа
    TypeTuple* NewTuple();
    // Создание структуры
    StructType* NewStruct();

    //------------------------------------------------------------------
    // Формирование объявления для прототипа функции
    DeclarationFunc* newDeclarationFuncProto(FuncProto* f);
    
    //------------------------------------------------------------------
    // Добавление в таблицу экспорта очередного объявления
    bool AddToExportNameTable(Declaration* dcl);
    // Формирование и добавление в таблицу имен предопределенных функций (операций)
    void AddPreFuncProtoToNameTable();
    // Сохранение таблицы экспорта после компиляции артефакта
    void Store(std::ofstream &ost);
    
    // Поиск объявления типа по его имени среди существующих таблиц имен
    DeclarationType* FindTypeDeclaration(std::string& n);
    
    // Поиск объявления в таблице экспорта по имени
    Declaration* FindExportedDeclaration(std::string& n);
    
    // Установка указателя на компилируемую функцию
    void SetFuncDefinition(Func* pf) {compiledFunction = pf;}
    
    //------------------------------------------------------------------
    // Получение указателей на предопределенные базовые функции
    // и другие предопределенные артефакты

    // Получение объявления функции ++
    DeclarationFunc* GetDeclarationInc();
    // Получение объявления функции +
    DeclarationFunc* GetDeclarationAdd();
    // Получение объявления функции --
    DeclarationFunc* GetDeclarationDec();
    // Получение объявления функции -
    DeclarationFunc* GetDeclarationSubMin();
};

#endif
